#include "game.h"
#include <ncurses.h>
#include "util.h"
#include <malloc.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Generate a specific number of businesses
struct Business *populateBusinesses(int count) {
    struct Business *businesses = malloc(sizeof(struct Business) * count);

    for (int i = 0; i < count; i++) {
        // Create random business
        snprintf(&businesses[i].name, sizeof(businesses[i].name), "%s", get_rand_name(TARGET_BUSINESS));
        businesses[i].balance = rand() % 10000;
        businesses[i].total_employees = 0;
        businesses[i].employees = NULL;
        businesses[i].properties = NULL;
        businesses[i].revenue = rand() % 600;
        businesses[i].expenses = 0.0;
        businesses[i].production_capacity = 100;
    }


    return businesses;
}

// Generate a number of properties
struct Property *populateProperties(int count) {
    struct Property *properties = malloc(sizeof(struct Property) * count);
    srand(time(NULL) * count);


    for (int i = 0; i < count; i++) {
        // Generate random property
        snprintf(properties[i].name, sizeof(properties[i].name), "%s", get_rand_name(TARGET_PROPERTY));
        double productivity = rand() % 10000;
        properties[i].productivity_bonus = productivity;
        properties[i].capacity = rand() % 10;
        properties[i].value = productivity * ( 1 + (rand() % 2));
        properties[i].maintenance_cost = properties[i].productivity_bonus / 10;
    }

    return properties;
}

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
            .revenue = 300.0,                  // Placeholder revenue
            .expenses = 0.0,                 // Placeholder expenses
            .production_capacity = 100          // Placeholder production capacity
        },
        .market_share = 5.0,                    // Initial market share
        .reputation = 50,                       // Initial reputation (on a 0-100 scale)
        .level = 1,                             // Starting level of the business
        .stock_value = 200.0                    // Placeholder stock value
    };
    const int propertyCount = 59;
    const int businessCount = 30;
    struct Business *businesses = populateBusinesses(businessCount); // all businesses in the world
    struct Property *properties = populateProperties(propertyCount); // Public pool of properties
    int selectedIndex = 0;
    int selectedPage = 0;
    size_t currentDay = 0;
    float timeFromLastDay = 0;
    float frameTime = 0.0f; // Time in milliseconds between each game loop
    struct timeval frameStart, frameEnd;
    int pageSize = 10;

    // Standard game menus
    struct Menu *activeMenu;
    struct Menu mainMenu;
    struct Menu propertyMenu;
    struct Menu businessMenu;

    // Main Menu
    INIT_MENU(mainMenu, 4);
    ADD_MENU_ENTRY(mainMenu, 0, OPEN, TARGET_MENU, &propertyMenu, "Buy Property"); // Open properties menu button
    ADD_MENU_ENTRY(mainMenu, 1, OPEN, TARGET_BUSINESS, &propertyMenu, "Manage Property"); // Open property management menu button
    ADD_MENU_ENTRY(mainMenu, 2, OPEN, TARGET_MENU, &businessMenu, "View Rival Companies"); // View other companies
    ADD_MENU_ENTRY(mainMenu, 3, OPEN, TARGET_BUSINESS, &player.business, "Customize Company"); // Customization options, e.g name

    // Properties Menu(s)
    INIT_MENU(propertyMenu, propertyCount + 1);
    for (int i = 0; i < propertyCount; i++) {
        ADD_MENU_ENTRY(propertyMenu, i, BUY, TARGET_PROPERTY, &properties[i], properties[i].name);
    }

    // Business Menu(s)
    INIT_MENU(businessMenu, businessCount + 1);
    for (int i = 0; i < businessCount; i++) {
        ADD_MENU_ENTRY(businessMenu, i, OPEN, TARGET_BUSINESS, &businesses[i], businesses[i].name);
    }


    // Windows
    WINDOW *controlsWindow = create_newwin(3, COLS, LINES - 3, 0, SCREEN_COLOR);
    WINDOW *detailsWindow = create_newwin(LINES - 3, COLS / 2, 0, COLS / 2, SCREEN_COLOR);
    WINDOW *mainWindow = create_newwin(LINES - 3, COLS / 2, 0, 0, SCREEN_COLOR);

    // Set active menu
    activeMenu = &mainMenu;

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

    // Prev Page
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Prev Page: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    waddch(controlsWindow, ACS_LARROW);
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));

    // Next Page
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Next Page: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    waddch(controlsWindow, ACS_RARROW);
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));


    // Exit to Menu Control
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Exit Game: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    wprintw(controlsWindow, "ESC");
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));

    // Exit to Menu Control
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Main Menu: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    wprintw(controlsWindow, "TAB");
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));

    // Activate Control
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Activate: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    wprintw(controlsWindow, "ENTER ");
    wattroff(controlsWindow, COLOR_PAIR(KEY_COLOR));
    // Save Control
    wattron(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));
    wprintw(controlsWindow, " Save: ");
    wattroff(controlsWindow, COLOR_PAIR(CONTROLS_SCREEN_COLOR));

    wattron(controlsWindow, COLOR_PAIR(KEY_COLOR));
    wprintw(controlsWindow, "F8");
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
        gettimeofday(&frameStart, NULL);
        int pageSelectedIndex = selectedIndex + pageSize * selectedPage;
        int currentPageSize;

        if (timeFromLastDay > 5000) {
            timeFromLastDay = 0;
            currentDay += 1;

            /* Daily Events */
            player.business.balance += (player.business.revenue - player.business.expenses);

        }

        /* Details Menu */

        // Clear Previous lines
        for (int i = 1; i < (dw_lines / 2); i++) {
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
        // Revenue
        mvwprintw(detailsWindow, 4, 1, "Revenue $%.0f", player.business.revenue);
        // Expenses
        mvwprintw(detailsWindow, 5, 1, "Expenses $%.0f", player.business.expenses);
        wattroff(detailsWindow, COLOR_PAIR(STAT_TEXT_COLOR));
        wattroff(detailsWindow, A_BOLD);

        /* Selected Stats */
        wattron(detailsWindow, COLOR_PAIR(STAT_SELECT_TEXT_COLOR));
        wattron(detailsWindow, A_BOLD);

        TargetType entryTarget = activeMenu->entries[pageSelectedIndex].target_type;
        int ss_line = dw_lines / 2;

        switch (entryTarget) {
            struct Business selectedBusiness;
            struct Property selectedProperty;

            case TARGET_BUSINESS:
                selectedBusiness = *activeMenu->entries[pageSelectedIndex].target.target_business;
                mvwprintw(detailsWindow, ss_line + 1, 1, "Name: %s", selectedBusiness.name);
                mvwprintw(detailsWindow, ss_line + 2, 1, "Balance: $%.0f", selectedBusiness.balance);
                mvwprintw(detailsWindow, ss_line + 3, 1, "Production: %d", selectedBusiness.production_capacity);
                mvwprintw(detailsWindow, ss_line + 4, 1, "Properties: %d", selectedBusiness.properties);
                break;
            case TARGET_PROPERTY:
                selectedProperty = *activeMenu->entries[pageSelectedIndex].target.target_property;
                mvwprintw(detailsWindow, ss_line + 1, 1, "Name: %s", selectedProperty.name);
                mvwprintw(detailsWindow, ss_line + 2, 1, "Value: %d", selectedProperty.value);
                mvwprintw(detailsWindow, ss_line + 3, 1, "Productivity: %d", selectedProperty.productivity_bonus);
                mvwprintw(detailsWindow, ss_line + 4, 1, "Upkeep: %d", selectedProperty.maintenance_cost);
                break;
            default:
                break;
        }

        wattroff(detailsWindow, COLOR_PAIR(STAT_SELECT_TEXT_COLOR));
        wattroff(detailsWindow, A_BOLD);

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
        for (int i = 0; (i + pageSize * selectedPage) < activeMenu->entry_num && i < pageSize; i++) {
            int index = i + pageSize * selectedPage;
            int menu_width = mw_cols - 15;
            char spaces[menu_width];
            int text_size = strlen(activeMenu->entries[index].name);
            memset(spaces, ' ', menu_width - text_size);
            spaces[(menu_width - text_size)] = '\0';

            if (selectedIndex == i) wattron(mainWindow, COLOR_PAIR(ENTRY_SELECT_COLOR));
            mvwprintw(mainWindow, 1 + i, 1, "%s%s", activeMenu->entries[index].name, spaces);
            if (selectedIndex == i) wattroff(mainWindow,COLOR_PAIR(ENTRY_SELECT_COLOR));
            currentPageSize++;
        }

        if (activeMenu->entry_num > pageSize) {
            mvwprintw(mainWindow, mw_lines - 2, 1, "Page %d of %d", selectedPage + 1, ((activeMenu->entry_num + 1) / pageSize));
        }

        // Refresh window
        wrefresh(mainWindow);

        // Handle Input
        timeout(1);
        int key = getch();
        struct MenuEntry selectedEntry;

        // Used for saving
        struct saveData data = {
            .player = player,
            .businesses = businesses,
            .properties = properties,
            .currentDay = currentDay,
            .business_num = businessCount,
            .property_num = propertyCount
        };
        char * filename[1024];
        int pageEntriesMax = pageSize;
        // Make sure we dont divide by zero (very scary)
        if (selectedPage > 0) {
            activeMenu->entry_num % selectedPage;
            if (pageEntriesMax > pageSize) pageEntriesMax = pageSize;
        }

        if (pageEntriesMax > activeMenu->entry_num) pageEntriesMax = activeMenu->entry_num;

        switch (key) {
            case KEY_DOWN:
                if (selectedIndex + 1 > activeMenu->entry_num - 1 || selectedIndex + 1 > pageEntriesMax - 1) selectedIndex = 0;
                else selectedIndex += 1;
                break;
            case KEY_UP:
                if (selectedIndex - 1 < 0) selectedIndex = pageEntriesMax - 1;
                else selectedIndex -= 1;
                break;
            case KEY_LEFT:
                if (selectedPage - 1 < 0) selectedPage = ((activeMenu->entry_num + 1) / pageSize);
                else selectedPage -=1;
                selectedIndex = 0;
                break;
            case KEY_RIGHT:
                if (selectedPage + 1 > ((activeMenu->entry_num + 1) / pageSize) - 1) selectedPage = 0;
                else selectedPage +=1;
                selectedIndex = 0;
                break;
            case 10:
                selectedEntry = activeMenu->entries[selectedIndex];
    
                switch (selectedEntry.action) {
                    case OPEN:
                        if (selectedEntry.target_type == TARGET_MENU) {
                            selectedIndex = 0;
                            selectedPage = 0;
                            activeMenu = selectedEntry.target.target_menu;
                        }
                        break;
                    default:
                        break;
                }

                break;
            case 27:
                delwin(controlsWindow);
                delwin(detailsWindow);
                delwin(mainWindow);
                timeout(-1);

                // Free variables
                DESTROY_MENU(mainMenu);
                DESTROY_MENU(propertyMenu);

                return;
                break;
            case KEY_F(8):
                // Save game
                snprintf(filename, sizeof(filename), "%s-%zu", player.business.name, currentDay);
                write_save(data, filename);

                break;
            case 9:
                selectedIndex = 0;
                selectedPage = 0;
                activeMenu = &mainMenu;
                break;
        }

        gettimeofday(&frameEnd, NULL);

        // Calculate the time taken for this frame
        frameTime = ((frameEnd.tv_sec - frameStart.tv_sec) * 1000000 + frameEnd.tv_usec - frameStart.tv_usec) / 1000;
        timeFromLastDay += frameTime;
    }
}