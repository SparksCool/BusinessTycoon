#include "mainmenu.h"
#include "util.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define BUTTON_COLOR 2
#define BUTTON_SELECT_COLOR 3

// Menu Options
typedef enum {
    START = 0,
    SAVES = 1,
    SETTINGS = 2,
    QUIT = 3
} MenuOption;


WINDOW *make_btn(const char * text, int position);

// Init the main menu
void menu_main() {
    WINDOW *startButton, *savesButton, *settingsButton, *quitButton;
    MenuOption selectedIndex = START;

    // Setup button colors
    init_pair(BUTTON_COLOR, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(BUTTON_SELECT_COLOR, COLOR_MAGENTA, COLOR_BLACK);

    // Create buttons
    startButton = make_btn("START", 0);
    savesButton = make_btn("LOAD", 1);
    settingsButton = make_btn("CONFIG", 2);
    quitButton = make_btn("EXIT", 3);

    // Array to make use of selection index easier
    WINDOW *buttons[] = {startButton, savesButton, settingsButton, quitButton};

    // Initial Highlighting
    wbkgd(buttons[selectedIndex], BUTTON_SELECT_COLOR);
    wrefresh(buttons[selectedIndex]);

    // Process Input
    while (1) {
        int key = getch();

        // Corrected info_log call
        char logMessage[50];
        snprintf(logMessage, sizeof(logMessage), "Key Pressed: %c", (char)key);
        info_log(logMessage);

        switch (key) {
            // The user has pressed the up arrow
            case KEY_UP:
                // Update old selected button color to be normal
                wbkgd(buttons[selectedIndex], COLOR_PAIR(BUTTON_COLOR));
                wrefresh(buttons[selectedIndex]);
                // Change selected index while remaining inside bounds of enum
                if (selectedIndex == START) {
                    selectedIndex = QUIT;
                } else {
                    selectedIndex -= 1;
                }
                // Update new selected button to have the right color
                wbkgd(buttons[selectedIndex], COLOR_PAIR(BUTTON_SELECT_COLOR));
                wrefresh(buttons[selectedIndex]);
                break;
            // The user has pressed the down arrow
            case KEY_DOWN:
                // Update old selected button color to be normal
                wbkgd(buttons[selectedIndex], COLOR_PAIR(BUTTON_COLOR));
                wrefresh(buttons[selectedIndex]);
                // Change selected index while remaining inside bounds of enum
                if (selectedIndex == QUIT) {
                    selectedIndex = START;
                } else {
                    selectedIndex += 1;
                }
                // Update new selected button to have the right color
                wbkgd(buttons[selectedIndex], COLOR_PAIR(BUTTON_SELECT_COLOR));
                wrefresh(buttons[selectedIndex]);
                break;
            case 10:
                // User has activated selected button
                switch (selectedIndex) {
                    case START:
                        break;
                    case SAVES:
                        break;
                    case SETTINGS:
                        break;
                    case QUIT:
                        for (int i = 0; i < 4; i++) {
                            delwin(buttons[i]);
                        }
                        endwin();
                        exit(0);
                        break;
                }
                break;
            case 27:
                for (int i = 0; i < 4; i++) {
                    delwin(buttons[i]);
                }
                endwin();

                exit(0);
                break;

        }
    }
    // TODO make clean up function
    // Clean up
    for (int i = 0; i < 4; i++) {
        delwin(buttons[i]);
    }
    endwin();

}

// Create new menu button at correct position with text
WINDOW *make_btn(const char * text, int position) {
    int width = 14;
    WINDOW *win = create_newwin(3, width, 1 + (4*position), (COLS - width) / 2, BUTTON_COLOR);
    mvwprintw(win, 1, (width - strlen(text)) / 2, text);
    wrefresh(win);
    return win;
}