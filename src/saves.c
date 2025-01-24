#include "saves.h"
#include "util.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>


    /*TODO:
    * Create save menu object, similar to game menu || DONE
    * Create a save entry class || DONE
    * Do not hold all save data in memory, I feel like this could be bad for memory || DONE
    * Use a page system, ideally find some good way to only load a piece of the save file names
    * Function to populate save menu || DONE
    */

void saves_menu() {
    char** saveNames = get_saves();

    // Windows
    WINDOW *controlsWindow = create_newwin(3, COLS, LINES - 3, 0, SCREEN_COLOR);
    WINDOW *detailsWindow = create_newwin(LINES - 3, COLS / 2, 0, COLS / 2, SCREEN_COLOR);
    WINDOW *mainWindow = create_newwin(LINES - 3, COLS / 2, 0, 0, SCREEN_COLOR);

    // Menu objects
    struct SaveMenu saveMenuObj = populateMenu();
    struct SaveMenu *saveMenu = &saveMenuObj;
    int selectedIndex;

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

        // Draw saves menu
        for (int i = 0; i < saveMenu->menuSize; i++) {

            // Center the text within the main window
            int menu_width = mw_cols - 15;
            char spaces[menu_width];
            int text_size = strlen(saveMenu->entries[i].saveName);
            memset(spaces, ' ', menu_width - text_size);
            spaces[(menu_width - text_size)] = '\0';

            mvwprintw(mainWindow, 1 + i, 1, "%s%s", saveMenu->entries[i].saveName, spaces);
        }

        // Refresh
        wrefresh(mainWindow);

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

struct SaveMenu populateMenu() {
    struct SaveMenu newMenu;
    struct SaveEntry* saveEntries = malloc(sizeof(saveEntries) * 1024);
    char** saves = get_saves();
    int size = 0;


    for (size_t i = 0; saves[i] != NULL; i++) {
        strcpy(saveEntries[i].saveName, saves[i]);
        size++;
    }


    newMenu.entries = saveEntries;
    newMenu.menuSize = size;

    return newMenu;
}

char** get_saves() {
    char** saveNames = malloc(sizeof(char*) * 1024); // Allocate memory for up to 1024 file names
    if (!saveNames) {
        perror("Failed to allocate memory");
        return NULL;
    }
    
    DIR *d = opendir("./saves");
    struct dirent *dir;
    size_t i = 0;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            // Skip `.` and `..`
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }
            saveNames[i] = strdup(dir->d_name); // Allocate and copy the file name
            if (!saveNames[i]) {
                perror("Failed to allocate memory for file name");
                closedir(d);
                // Free previously allocated memory
                for (size_t j = 0; j < i; ++j) {
                    free(saveNames[j]);
                }
                free(saveNames);
                return NULL;
            }
            i++;
        }
        closedir(d);
    }

    saveNames[i] = NULL; // Null-terminate the array
    return saveNames;
}