#include "main.h"
#include "util.h"
#include "mainmenu.h"
#include <ncurses.h>
#include <string.h>

WINDOW *create_newwin(int height, int width, int starty, int startx, int color);
void destroy_win(WINDOW *local_win);

int main(void) {
    // Variables
    int ch;
    WINDOW *main_window;

    // Initialize ncurses
    initscr();
    // Dont echo when getch is used
    noecho();
    // Enable usage of keys like F1, F2, etc
    keypad(stdscr, TRUE);
    // Disable line buffering
    raw();
    // Disable cursor
    curs_set(0);

    if (!has_colors()) {
        endwin();
        printf("Color is not supported in this terminal!");
        return 1;
    }

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);

    refresh();
    // Create main window
    main_window = create_newwin(LINES - 1, COLS - 1, 0, 0, 1);

    // Start main menu
    menu_main();

    wrefresh(main_window);
    getch();
    endwin();

    return 0;
}