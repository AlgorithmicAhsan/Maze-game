#include "classes.h"
int main() 
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    grid g;
    while (true)
    {
        int menuChoice = g.showMenu();
        if (menuChoice == 2) break;
        
        int choice = g.chooseDifficulty();
        int gSize;
        switch(choice) {
            case 1: gSize = 10; break;
            case 2: gSize = 15; break;
            case 3: gSize = 20; break;
        }
        
        g = grid(gSize);
        g.gridCreation();

        char ch;
        ch =' ';
        while (ch != 'q' && ch != 27)
        {
            g.display();
            ch = getch();
            if (ch == 'p')
            {
                g.isPaused = !g.isPaused;
                if (g.isPaused)
                {
                    clear();
                    mvprintw(gSize/2, gSize - 5, "GAME PAUSED");
                    refresh();
                }
            }
            else if (!g.isPaused && (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd'))
            {
                g.movePlayer(ch);
            }
        }
        if (ch == 27)
        break;
    }
    endwin();
    return 0;
}