#ifndef UTIL_H
#define UTIL_H
#include <ncurses.h>

WINDOW *create_newwin(int height, int width, int starty, int startx, int color);
void destroy_win(WINDOW *local_win);
#endif // UTIL_H