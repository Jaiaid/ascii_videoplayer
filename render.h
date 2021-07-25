#ifndef IMGPROC_H
#define IMGPROC_H

#include <string>
#include <opencv2/videoio.hpp>

namespace render
{	
	const int DEFAULT_FPS = 30;
	const int SEEK_STEP_SEC = 2; 
	
	enum render_state
	{
		INIT,
		OPEN,
		START,
		PAUSE,
		CLOSE,
	};
	
	enum render_type
	{
		IMAGE,
		VIDEO,
		NOT_OPENED
	};
		
	struct render_loop_arg_struct
	{
		render_state* state_ptr;
		render_type* type_ptr;
		int* forward_request_count_ptr;
		int* backward_request_count_ptr;
		cv::VideoCapture* stream_ptr;
	};

	void frame_skip();
	void* render_loop(void* arg);
		
	class Renderer
	{	
		std::string src_name;
		render_state state;
		render_type type;

		// use to indicate to render thread that frame skip needed
		// can be set by forward and backward method
		int forward_request_count;
		int backward_request_count;

		cv::VideoCapture stream;
		
		render_loop_arg_struct render_infos;
		pthread_t render_thread;
		
		void type_resolve();
	public:
		Renderer();
		bool is_image();
		bool is_video();
		bool is_closed();
		bool open(char* src_name);
		void start();
		void toggle();
		void pause();
		void resume();
		void forward_request();
		void backward_request();
		void close();
	};
}

#endif
