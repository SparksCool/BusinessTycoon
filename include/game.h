#ifndef GAME_H
#define GAME_H

struct Employee {
    char name[50];           // Name of the employee
    int id;                  // Unique employee ID
    double salary;           // Salary of the employee
    int efficiency;          // Efficiency rating (0-100)
    int happiness;           // Happiness level (0-100)
    char position[30];       // Employee's position or role
};

struct Property {
    char name[50];           // Name of the property (e.g., Office, Factory)
    double value;            // Value of the property
    int capacity;            // Capacity (e.g., how many employees it can house)
    double maintenance_cost; // Cost of maintaining the property
    int productivity_bonus;  // Bonus to productivity if this property improves production
};

struct Business {
    char name[50];           // Business name
    double balance;          // Current financial balance
    int total_employees;     // Total number of employees
    struct Employee *employees;  // Array of employees
    struct Property *properties; // Array of properties owned by the business
    int num_properties;      // Total number of properties owned
    double revenue;          // Revenue generated per cycle (e.g., per turn)
    double expenses;         // Total expenses (employee salaries, maintenance costs, etc.)
    int production_capacity; // Total production capacity based on employees and properties
};

struct PlayerBusiness {
    struct Business business;    // Player's business structure
    double market_share;         // Player’s market share (percentage)
    int reputation;              // Business reputation (0-100 scale)
    int level;                   // Level or rank of the player’s business
    double stock_value;          // Stock value of the business (for public companies)
};

typedef enum {
    BUY,                    // Used for buying companies or property
    SELL,                   // Used for selling companies or property
    OPEN,                   // Used for opening any kind of object to see its menu and stats
    FIRE,                   // Used for firing employees
    HIRE,                   // Used for hiring employees
    UPGRADE,                // Used for upgrading properties
    BACK,                   // Get back to previous menu
    NONE                    // Used for buttons that cant be used
} MenuAction;

typedef enum {
    TARGET_NONE,
    TARGET_BUSINESS,
    TARGET_PROPERTY,
    TARGET_EMPLOYEE,
    TARGET_MENU
} TargetType;

struct Menu;

struct MenuEntry {
    union {
        struct Business *target_business;
        struct Property *target_property; 
        struct Employee *target_employee;
        struct Menu  *target_menu;
    } target;
    TargetType target_type; // Determines target type
    MenuAction action; // Default OPEN, Determines type of action to take on target
    char name[50];
};

struct Menu {
    int entry_num; // Denotes number of entries
    struct MenuEntry *entries; // List of all entries
};

#define MENU_ENTRY_MEMORY 128
// Initialize a new menu
#define INIT_MENU(menu, num_entries) \
    menu.entry_num = num_entries; \
    menu.entries = malloc(MENU_ENTRY_MEMORY * num_entries); \
    if (menu.entries == NULL) {perror("Failed to allocate memory for menu entries"); exit(1);}

// Free all entries in a menu
#define DESTROY_MENU(menu) \
    free(menu.entries);

// Add a new menu entry
#define ADD_MENU_ENTRY(menu, index, action_, target_type_, target_, name_) \
    snprintf(menu.entries[index].name, sizeof(menu.entries[index].name), name_); \
    menu.entries[index].action = action_; \
    if (target_type_ == TARGET_BUSINESS) menu.entries[index].target.target_business = (struct Business *)(target_); \
    else if (target_type_ == TARGET_PROPERTY) menu.entries[index].target.target_property = (struct Property *)(target_); \
    else if (target_type_ == TARGET_EMPLOYEE) menu.entries[index].target.target_employee = (struct Employee *)(target_); \
    else if (target_type_ == TARGET_MENU) menu.entries[index].target.target_menu = (struct Menu *)(target_); \
    else menu.entries[index].target.target_employee = NULL; \
    menu.entries[index].target_type = target_type_;

// Free a single menu entry
#define DESTROY_MENU_ENTRY(menu, index) \
    free(menu.entries[index].name);

void game_main();
#endif