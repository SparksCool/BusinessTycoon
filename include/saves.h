#ifndef SAVES_H
#define SAVES_H

char** get_saves();

void saves_menu();

struct SaveMenu populateMenu();

struct SaveEntry {
    char saveName[256]; // Save Name
};

struct SaveMenu {
    struct SaveEntry *entries; // The saves contained in the menu
    int menuSize; // The size of this menu
};



#endif