#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>
#include <time.h>
#include <pthread.h>
#include <ncurses.h>

#include "color.h"
#include "render.h"

render::Renderer::Renderer()
{
	this->state = render::render_state::INIT;
}

bool render::Renderer::is_image()
{
	return this->type == render::render_type::IMAGE;
}

bool render::Renderer::is_video()
{
	return this->type == render::render_type::VIDEO;
}

bool render::Renderer::is_closed()
{
	return this->state == render::render_state::CLOSE;
}

bool render::Renderer::open(char* src_name)
{
	bool ok = this->stream.open(src_name);
	if (ok) {
		this->src_name = std::string(src_name);
		this->state = render::render_state::OPEN;
	}
	
	return ok;
}

void render::Renderer::start()
{
	this->render_infos.state_ptr = &(this->state);
	this->render_infos.type_ptr = &(this->type);
	this->render_infos.stream_ptr = &(this->stream);
	
	pthread_create(&render_thread, NULL, render_loop, &(this->render_infos));
	this->state = render::render_state::START;
}

void render::Renderer::toggle()
{
	if (this->state == render::render_state::START) {
		this->state = render::render_state::PAUSE;
	}
	else {
		this->state = render::render_state::START;
	}
}

void render::Renderer::pause()
{
	this->state = render::render_state::PAUSE;
}

void render::Renderer::resume()
{
	this->state = render::render_state::START;
}

void render::Renderer::close()
{
	this->state = render::render_state::CLOSE;
	// wait until render loop is closed
	pthread_join(this->render_thread, NULL);
}

void* render::render_loop(void* arg)
{
	render::render_state* state_flag_ptr = ((render::render_loop_arg_struct*)arg)->state_ptr;
	render::render_type* type_flag_ptr = ((render::render_loop_arg_struct*)arg)->type_ptr;
	cv::VideoCapture* stream_ptr = ((render::render_loop_arg_struct*)arg)->stream_ptr;

	int term_h, term_w;
	int fps;
	int per_frame_millisec;
	cchar_t cchar_to_print; 
	wchar_t wchar_to_print;
	struct timespec req, rem;
	cv::Mat frame, resized_frame, render_frame;

	raw();
	noecho();
	curs_set(0);
	getmaxyx(stdscr, term_h, term_w);
	
	fps = stream_ptr->get(cv::CAP_PROP_FPS);
	if (fps == 0) {
		fps = render::DEFAULT_FPS;
	}
	per_frame_millisec = 1000/fps;
	
	std::chrono::steady_clock::time_point render_start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point render_finish;
	
	bool ok = stream_ptr->read(frame);
	while(*state_flag_ptr != render::render_state::CLOSE && ok)
	{
		cv::resize(frame, resized_frame, cv::Size(term_w, term_h), 0, 0, cv::INTER_AREA);
		cv::cvtColor(resized_frame, render_frame, cv::COLOR_BGR2HSV);
	
		unsigned char* frame_ptr = (unsigned char*)render_frame.data;
		move(0, 0);
		for(int y = 0;y < term_h;y++)
		{
			for(int x = 0;x < term_w;x++)
			{
				cv::Vec3b& element = render_frame.at<cv::Vec3b>(y, x);
				int hue = element[0]; 
				int sat = element[1]; 
				int val = element[2];
				int color_pair_id = get_color_pair_id(hue, val);
				wchar_t wchar_to_print = get_fg_mask(sat);
				
				attron(COLOR_PAIR(color_pair_id));
				setcchar(&cchar_to_print, &wchar_to_print, 0, COLOR_PAIR(color_pair_id), NULL);
				add_wch(&cchar_to_print);
			}
		}
		refresh();
		
		render_finish = std::chrono::steady_clock::now();
		int millisec_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(render_finish - render_start).count();
		
		if (millisec_elapsed < per_frame_millisec) {
			req.tv_sec = 0;
			req.tv_nsec = (30 - millisec_elapsed) * 1e6;
			nanosleep(&req, &rem);
		}

		while (*state_flag_ptr == render::render_state::PAUSE)
		{
			req.tv_sec = 0;
			req.tv_nsec = 1e8;
			nanosleep(&req, &rem);
		}

		render_start = std::chrono::steady_clock::now();
		ok = stream_ptr->read(frame);
	}
	
	stream_ptr->release();
	*state_flag_ptr = render::render_state::CLOSE;
}
