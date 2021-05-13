#ifndef COLOR_H
#define COLOR_H

#include <ncurses.h>

#define RED 8
#define YELLOW 10
#define GREEN 12
#define CYAN 14
#define BLUE 16
#define MAGENTA 18
#define WHITE 0
#define BLACK 1

// following encoding is used

#define RED_WHITE 8
#define RED_BLACK 9
#define YELLOW_WHITE 10
#define YELLOW_BLACK 11
#define GREEN_WHITE 12
#define GREEN_BLACK 13
#define CYAN_WHITE 14
#define CYAN_BLACK 15
#define BLUE_WHITE 16
#define BLUE_BLACK 17
#define MAGENTA_WHITE 18
#define MAGENTA_BLACK 19


// foreground mask using extended ascii code to create gray value effect
const wchar_t fg_mask[5] = 
{
	L'█', L'▓', L'▒', L'░', L' '
	//L'\u219', L'\u178', L'\u177', L'\u176', L' '
};

// background mask color determines what will be the bg color of character cell 
unsigned int get_bg_mask_color(int hue);

// foreground mask color determines what will be the fg color of character cell
unsigned int get_fg_mask_color(int val);

// fg mask determines which character will be printed in a cell
wchar_t get_fg_mask(int sat);

// to resolve color pair, 
int get_color_pair_id(int hue, int val);

void init_colors();
#endif
