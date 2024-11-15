#include "saves.h"
#include "util.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>


void saves_menu() {
    char* saveNames = get_saves();

        // Windows
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
    wprintw(controlsWindow, " Exit: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    wprintw(controlsWindow, "ESC");
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));

    // Activate Control
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Activate: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    wprintw(controlsWindow, "ENTER ");
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));


    // Refresh
    wrefresh(controlsWindow);

    // Setup details window
    int dw_cols, dw_lines;
    getmaxyx(detailsWindow, dw_lines, dw_cols);

    // Refresh
    wrefresh(detailsWindow);

    // Setup main window
    int mw_cols, mw_lines;
    getmaxyx(mainWindow, mw_lines, mw_cols);

    // Refresh
    wrefresh(mainWindow);

    // Window loop
    while (1) {
        // Handle Input
        timeout(1);
        int key = getch();

        switch (key) {
            case 27:
                delwin(controlsWindow);
                delwin(detailsWindow);
                delwin(mainWindow);
                timeout(-1);

                return;
                break;
        }
    }
}

char* get_saves() {
    char* saveNames[512];
    DIR *d;
    struct dirent *dir;
    d = opendir("./saves");
    size_t i = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            strcpy(saveNames[i], dir->d_name);
        }
    }

    return saveNames;
}