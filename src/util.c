#include "util.h"
#include <ncurses.h>

// Create new window
WINDOW *create_newwin(int height, int width, int starty, int startx, int color) {
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);

    wbkgd(local_win, COLOR_PAIR(color));
    box(local_win, 0, 0);
    wrefresh(local_win);

    return local_win;
}

// Destroy window
void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

	wrefresh(local_win);
	delwin(local_win);
}
