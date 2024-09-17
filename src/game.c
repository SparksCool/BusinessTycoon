#include "game.h"
#include <ncurses.h>
#include "util.h"
#include <malloc.h>
#include <string.h>


void game_main() {
    // Game variables
    struct PlayerBusiness player = {
        .business = {
            .name = "Player Company",           // Placeholder name
            .balance = 10000.0,                 // Initial financial balance
            .total_employees = 10,              // Number of employees
            .employees = NULL,                  // No employees yet
            .properties = NULL,                 // No properties yet
            .num_properties = 0,                // Number of properties
            .revenue = 5000.0,                  // Placeholder revenue
            .expenses = 3000.0,                 // Placeholder expenses
            .production_capacity = 100          // Placeholder production capacity
        },
        .market_share = 5.0,                    // Initial market share
        .reputation = 50,                       // Initial reputation (on a 0-100 scale)
        .level = 1,                             // Starting level of the business
        .stock_value = 200.0                    // Placeholder stock value
    };
    struct Business businesses[50] = {}; // will be loaded by JSON file preset
    struct Property properties[200] = {}; // will be loaded by JSON file preset
    int selectedIndex = 0;

    // Standard game menus
    struct Menu activeMenu;
    struct Menu mainMenu;
    
    // Main Menu
    INIT_MENU(mainMenu, 2);
    ADD_MENU_ENTRY(mainMenu, 0, BUY_PROPERTIES, TARGET_NONE, &player.business, "Buy Property"); // Open properties menu button
    ADD_MENU_ENTRY(mainMenu, 1, PROPERTIES, TARGET_BUSINESS, &player.business, "Manage Property"); // Open property management menu button

    // Windows
    WINDOW *controlsWindow = create_newwin(3, COLS, LINES - 3, 0, SCREEN_COLOR);
    WINDOW *detailsWindow = create_newwin(LINES - 3, COLS / 2, 0, COLS / 2, SCREEN_COLOR);
    WINDOW *mainWindow = create_newwin(LINES - 3, COLS / 2, 0, 0, SCREEN_COLOR);

    // Set active menu
    activeMenu = mainMenu;

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

    // Setup details window
    int dw_cols, dw_lines;
    getmaxyx(detailsWindow, dw_lines, dw_cols);

    // Window title
    mvwprintw(detailsWindow, 0, (dw_cols - strlen("Company Stats")) / 2, "Company Stats");

    // Divider
    mvwhline(detailsWindow, dw_lines/2, 1, ACS_HLINE, dw_cols - 2);
    mvwprintw(detailsWindow, dw_lines/2, (dw_cols - strlen("Selected Stats")) / 2, "Selected Stats");

    // Refresh
    wrefresh(detailsWindow);

    // Setup main window
    int mw_cols, mw_lines;
    getmaxyx(mainWindow, mw_lines, mw_cols);

    // Refresh
    wrefresh(mainWindow);

    // Game loop
    while(1) {
        /* Details Menu */

        // Clear Previous lines
        for (int i = 1; i < (dw_lines / 2) - 5; i++) {
            wmove(detailsWindow, i, 1);
            wclrtoeol(detailsWindow);
            wmove(detailsWindow, (dw_lines / 2) + i, 1);
            wclrtoeol(detailsWindow);
        }
        // Restore window borders
        box(detailsWindow, 0, 0);
        // Window title
        mvwprintw(detailsWindow, 0, (dw_cols - strlen("Company Stats")) / 2, "Company Stats");

        // Attributes
        wattron(detailsWindow, COLOR_PAIR(STAT_TEXT_COLOR));
        wattron(detailsWindow, A_BOLD);

        /* Company Stats */

        // Company Name
        mvwprintw(detailsWindow, 1, 1, "Company: %s", player.business.name);
        // Properties
        mvwprintw(detailsWindow, 2, 1, "Properties: %d", player.business.num_properties);
        // Money
        mvwprintw(detailsWindow, 3, 1, "Balance: $%.0f", player.business.balance);

        wattroff(detailsWindow, COLOR_PAIR(STAT_TEXT_COLOR));
        wattroff(detailsWindow, A_BOLD);

        /* Selected Stats */

        // Refresh window
        wrefresh(detailsWindow);



        /* Primary Menu */

        // Clear Previous lines
        for (int i = 1; i < mw_lines - 1; i++) {
            wmove(mainWindow, i, 1);
            wclrtoeol(mainWindow);
        }
        // Restore window borders
        box(mainWindow, 0, 0);

        // Populate menu with active menu
        for (int i = 0; i < activeMenu.entry_num; i++) {
            int menu_width = mw_cols - 15;
            char spaces[menu_width];
            int text_size = strlen(activeMenu.entries[i].name);
            memset(spaces, ' ', menu_width - text_size);
            spaces[(menu_width - text_size)] = '\0';

            if (selectedIndex == i) wattron(mainWindow, COLOR_PAIR(ENTRY_SELECT_COLOR));
            mvwprintw(mainWindow, 1 + i, 1, "%s%s", activeMenu.entries[i].name, spaces);
            if (selectedIndex == i) wattroff(mainWindow,COLOR_PAIR(ENTRY_SELECT_COLOR));
        }

        // Refresh window
        wrefresh(mainWindow);

        // Handle Input
        timeout(1);
        int key = getch();
        switch (key) {
            case KEY_UP:
                if (selectedIndex + 1 > activeMenu.entry_num - 1) selectedIndex = 0;
                else selectedIndex += 1;
                break;
            case KEY_DOWN:
                if (selectedIndex - 1 < 0) selectedIndex = activeMenu.entry_num - 1;
                else selectedIndex -= 1;
                break;
            case 10:
                break;
            case 27:
                delwin(controlsWindow);
                delwin(detailsWindow);
                delwin(mainWindow);
                timeout(-1);

                // Free variables

                return;
                break;
        }
    
    }
}