#include <cstdio>
#include <ncurses.h>
#include <locale.h>

#include "color.h"
#include "render.h"

#define EXIT 'q'
#define PAUSE 'p'
#define FORWARD 'w'
#define BACKWARD 's'

bool quit = false;

int main(int argc, char **argv)
{
	if(argc != 2){
		printf("<video/image name>\n");
		return 0;
	}
	
	setlocale(LC_ALL, "");
	initscr();
	if (has_colors() == FALSE) {
		printf("terminal does not support color\n");
		endwin();
		exit(0);
	}
	start_color();

	if (can_change_color() == FALSE) {
		printf("terminal does not support color definition change\n");
	}
	// prepare colors
	init_colors();

	render::Renderer renderer;
	bool ok = renderer.open(argv[1]);
	if (ok) {
		renderer.start();

		char cmd;
		while(!quit && !renderer.is_closed())
		{
			cmd = getchar();
			switch(cmd)
			{
				case EXIT:
					quit = true;
					renderer.close();
					break;
				case PAUSE:
					renderer.toggle();
					break;
				case FORWARD:
					renderer.forward_request();
					break;
				case BACKWARD:
					renderer.backward_request();
			}
		}
	}
	// close the renderer
	renderer.close();

	endwin();
	if (!ok) {
		printf("failed to open stream from %s, exiting\n", argv[1]);
	}
	
	return 0;
}
