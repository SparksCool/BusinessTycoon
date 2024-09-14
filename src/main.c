#include "main.h"
#include <ncurses.h>
#include <string.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(void) {
    // Variables
    int ch;
    WINDOW *main_window;

    // Initialize ncurses
    initscr();
    // Dont echo when getch is used
    noecho();
    // Enable usage of keys like F1, F2, etc
    keypad(stdscr, TRUE);
    // Disable line buffering
    raw();

    if (!has_colors()) {
        endwin();
        printf("Color is not supported in this terminal!");
        return 1;
    }

    start_color();

    refresh();
    // Create main window
    main_window = create_newwin(LINES - 1, COLS - 1, 0, 0);

    ch = getch();

    if (ch == KEY_LEFT) {
        attron(A_BOLD);
        char mesg[] = "GAME READY!";
        mvwprintw(main_window, LINES/2, (COLS-strlen(mesg))/2,"%s", mesg);
        attroff(A_BOLD);
    } else {
        attron(A_ITALIC);
        char mesg[] = "Unhandled Input: ";
        mvwprintw(main_window, LINES/2, (COLS-strlen(mesg) + 1)/2,"%s%c", mesg, ch);
        attroff(A_ITALIC);
    }

    wrefresh(main_window);
    getch();
    endwin();

    return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);

    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    wbkgd(local_win, COLOR_PAIR(1));
    box(local_win, 0, 0);
    wrefresh(local_win);

    return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

	wrefresh(local_win);
	delwin(local_win);
}
