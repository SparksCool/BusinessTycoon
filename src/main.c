#include "main.h"
#include "util.h"
#include "mainmenu.h"
#include <ncurses.h>
#include <string.h>

WINDOW *create_newwin(int height, int width, int starty, int startx, int color);
void destroy_win(WINDOW *local_win);
WINDOW *main_window;

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

    // Setup Colors
    start_color();
    init_pair(SCREEN_COLOR, COLOR_BLACK, COLOR_BLUE);
    init_pair(BUTTON_SELECT_COLOR, COLOR_BLACK, COLOR_CYAN);
    init_pair(BUTTON_COLOR, COLOR_CYAN, COLOR_BLACK);
    init_pair(KEY_COLOR, COLOR_BLACK, COLOR_WHITE);
    init_pair(CONTROLS_SCREEN_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(STAT_TEXT_COLOR, COLOR_GREEN, COLOR_BLUE);
    init_pair(ENTRY_SELECT_COLOR, COLOR_BLUE, COLOR_BLACK);
    init_pair(STAT_SELECT_TEXT_COLOR, COLOR_RED, COLOR_BLUE);

    refresh();
    // Create main window
    main_window = create_newwin(LINES - 1, COLS - 1, 0, 0, SCREEN_COLOR);

    // Start main menu
    menu_main(main_window);

    wrefresh(main_window);
    getch();
    endwin();

    return 0;
}