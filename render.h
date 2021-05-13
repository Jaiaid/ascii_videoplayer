#ifndef IMGPROC_H
#define IMGPROC_H

#include <string>
#include <opencv2/videoio.hpp>

namespace render
{	
	const int DEFAULT_FPS = 30;
	
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
		cv::VideoCapture* stream_ptr;
	};

	void* render_loop(void* arg);
		
	class Renderer
	{	
		std::string src_name;
		render_state state;
		render_type type;
		
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
		void close();
	};
}

#endif
