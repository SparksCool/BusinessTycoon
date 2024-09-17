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

void info_log(const char * text) {
    // Clear remains of any previous logs
    move(LINES - 1, 0);
    clrtoeol();
    // Print new log
    mvwprintw(stdscr, LINES - 1, 0, text);
    wrefresh(stdscr);
}

struct saveData read_save(const char * filename) { 
    // Save data
    struct PlayerBusiness player;
    struct Business *businesses;
    struct Property *properties;
    int day;


    

    struct saveData data = {
        .player = player,
        .businesses = businesses,
        .properties = properties,
        .currentDay = day
    };

    return data;
}

void write_save(struct saveData data, const char * filename) {
    // Save data
    struct PlayerBusiness player = data.player;
    struct Business *businesses = data.businesses;
    struct Property *properties = data.properties;
    int day = data.currentDay;

    FILE *of;
    of = fopen(filename, "w");

    
}