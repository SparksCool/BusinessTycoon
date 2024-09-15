#include "game.h"
#include <ncurses.h>
#include "util.h"


void game_main() {
    WINDOW *controlsWindow = create_newwin(3, COLS, LINES - 3, 0, SCREEN_COLOR);
    WINDOW *detailsWindow = create_newwin(LINES - 3, COLS / 2, 0, COLS / 2, SCREEN_COLOR);
    WINDOW *mainWindow = create_newwin(LINES - 3, COLS / 2, 0, 0, SCREEN_COLOR);

    // Setup controls window
    wmove(controlsWindow, 1, 2);

    // Up Control
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Up: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    waddch(controlsWindow, ACS_UARROW);
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));

    // Down Control
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Down: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    waddch(controlsWindow, ACS_DARROW);
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));
    // Exit to Menu Control
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Exit to Menu: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    wprintw(controlsWindow, "ESC");
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));

    // Refresh
    wrefresh(controlsWindow);

    // Input loop
    while(1) {
        int key = getch();
        switch (key) {
            case 27:
                delwin(controlsWindow);
                delwin(detailsWindow);
                delwin(mainWindow);
                return;
                break;
        }
    }
}