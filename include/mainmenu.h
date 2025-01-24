#ifndef MAINMENU_H
#define MAINMENU_H
#include <ncurses.h>
void menu_main(WINDOW *main_window);
WINDOW *make_btn(const char * text, int position);
#endif // MENU_H