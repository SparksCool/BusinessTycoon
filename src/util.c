#include "util.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LINES 2048

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

void info_log(const char * text, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, text);
    vsnprintf(buffer, sizeof(buffer), text, args);
    va_end(args);

    // Clear remains of any previous logs
    move(LINES - 1, 0);
    clrtoeol();

    // Print new log
    mvwprintw(stdscr, LINES - 1, 0, buffer);
    wrefresh(stdscr);
}

// Read save data
struct saveData read_save(const char * filename_) { 
    // Save data
    struct PlayerBusiness player;
    struct Business *businesses;
    struct Property *properties;
    size_t day;
    size_t business_num;
    size_t property_num;
    char filename[256];

    snprintf(filename, sizeof(filename), "saves/%s", filename_);


    // Open file
    FILE *of;
    of = fopen(filename, "r");
    if (of == NULL) {info_log("Invalid file!"); struct saveData data = {}; return data;}

    // Read data
    fread(&player, sizeof(struct PlayerBusiness), 1, of);
    fread(&business_num, sizeof(size_t), 1, of);
    fread(&property_num, sizeof(size_t), 1, of);
    fread(businesses, sizeof(struct Business) * business_num, 1, of);
    fread(properties, sizeof(struct Property) * property_num, 1, of);
    if (&fread != 0) {
        info_log("Save loaded!");
    } else {
        info_log("Error reading file!");
    }

    struct saveData data = {
        .player = player,
        .businesses = businesses,
        .properties = properties,
        .currentDay = day,
        .business_num = business_num,
        .property_num = property_num
    };

    fclose(of);

    return data;
}

// Write save data
void write_save(struct saveData data, const char * filename_) {
    // Save data
    struct PlayerBusiness player = data.player;
    struct Business *businesses = data.businesses;
    struct Property *properties = data.properties;
    size_t day = data.currentDay;
    size_t business_num = data.business_num;
    size_t property_num = data.property_num;
    char filename[256];

    snprintf(filename, sizeof(filename), "saves/%s", filename_);

    // Create or overwrite file
    FILE *of;
    of = fopen(filename, "w+");
    if (of == NULL) {info_log("Invalid file!"); return;}

    // Write data
    fwrite(&player, sizeof(player), 1, of);
    fwrite(&business_num, sizeof(size_t), 1, of);
    fwrite(&property_num, sizeof(size_t), 1, of);
    fwrite(businesses, sizeof(businesses), 1, of);
    fwrite(properties, sizeof(properties), 1, of);
    fwrite(&day, sizeof(size_t), 1, of);
    if (&fwrite != 0) {
        info_log("Save %s created!", filename);
    } else {
        info_log("Error writing file!");
    }

    fclose(of);
}

char * get_rand_name(TargetType type) {
    char filename[48];
    char* names[MAX_NAME_LINES];
    size_t count = 0;
    unsigned int seed;

    switch (type) {
        case TARGET_BUSINESS:
            snprintf(filename, sizeof(filename), "resources/b_names");
            break;
        case TARGET_PROPERTY:
            snprintf(filename, sizeof(filename), "resources/p_names");
            break;
        case TARGET_EMPLOYEE:
            snprintf(filename, sizeof(filename), "resources/e_names");
            break;
        default:
            return "INVALID";
    }

    FILE *of = fopen(filename, "r");
    if (of == NULL) {
        perror("Resource file missing!");
        return "INVALID";
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), of) && count < MAX_NAME_LINES) {
        // Remove newline if present
        buffer[strcspn(buffer, "\n")] = 0;

        names[count] = malloc(strlen(buffer) + 1);
        if (names[count] == NULL) {
            perror("Memory allocation failed");
            fclose(of);
            return "INVALID";
        }
        // Should ensure more random results
        seed += rand() * count;
        
        strcpy(names[count], buffer);
        count++;
    }

    fclose(of);

    if (count == 0) {
        return "INVALID";
    }

    // Get a random name
    srand(time(NULL) * rand());
    char *random_name = names[rand() % count];

    // Free the other names
    for (size_t i = 0; i < count; i++) {
        if (names[i] != random_name) {
            free(names[i]);
        }
    }

    return random_name;    
}