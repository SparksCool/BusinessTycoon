#ifndef UTIL_H
#define UTIL_H
#include <ncurses.h>

#define SCREEN_COLOR 1
#define BUTTON_COLOR 2
#define BUTTON_SELECT_COLOR 3
#define KEY_COLOR 4
#define CONTROLS_SCREEN_COLOR 5

WINDOW *create_newwin(int height, int width, int starty, int startx, int color);
void destroy_win(WINDOW *local_win);
void info_log(const char * text);
#endif // UTIL_H