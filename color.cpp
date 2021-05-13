#include "color.h"

// background mask color determines what will be the bg color of character cell 
unsigned int get_bg_mask_color(int hue)
{
	// constants are for hue range 0-180 not for range 0-359
	if (hue < 15) {
		return RED;
	}
	else if (hue < 45) {
		return YELLOW;
	}
	else if (hue < 75) {
		return GREEN;
	}
	else if (hue < 105) {
		return CYAN;
	}
	else if (hue < 135) {
		return BLUE;
	}
	else if (hue < 165) {
		return MAGENTA;
	}
	
	return RED;
}

// foreground mask color determines what will be the fg color of character cell
unsigned int get_fg_mask_color(int val)
{
	// constants are for val range 0-255
	if (val < 128) {
		return BLACK;
	}
	return WHITE;
}

// fg mask determines which character will be printed in a cell
wchar_t get_fg_mask(int sat)
{	
	// 255/5 = 51 (integer division)
	if (sat < 51) {
		return fg_mask[0];
	}
	else if (sat < 102) {
		return fg_mask[1];
	}
	else if (sat < 153) {
		return fg_mask[2];
	}
	else if (sat < 204) {
		return fg_mask[3];
	}
	return fg_mask[4];
}

// to resolve color pair, 
int get_color_pair_id(int hue, int val)
{
	int fg_color = get_fg_mask_color(val);
	int bg_color = get_bg_mask_color(hue);
	
	return bg_color + fg_color;
}

void init_colors()
{
	init_pair(RED_WHITE, COLOR_WHITE, COLOR_RED);
	init_pair(RED_BLACK, COLOR_BLACK, COLOR_RED);
	init_pair(YELLOW_WHITE, COLOR_WHITE, COLOR_YELLOW);
	init_pair(YELLOW_BLACK, COLOR_BLACK, COLOR_YELLOW);
	init_pair(GREEN_WHITE, COLOR_WHITE, COLOR_GREEN);
	init_pair(GREEN_BLACK, COLOR_BLACK, COLOR_GREEN);
	init_pair(CYAN_WHITE, COLOR_WHITE, COLOR_CYAN);
	init_pair(CYAN_BLACK, COLOR_BLACK, COLOR_CYAN);
	init_pair(BLUE_WHITE, COLOR_WHITE, COLOR_BLUE);
	init_pair(BLUE_BLACK, COLOR_BLACK, COLOR_BLUE);
	init_pair(MAGENTA_WHITE, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(MAGENTA_BLACK, COLOR_BLACK, COLOR_MAGENTA);
}
