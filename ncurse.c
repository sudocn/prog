#include <ncurses.h>
#include <stdlib.h>

void vertical_bar(int y, int x, int h)
{
  int y_max = y + h;
  while (y < y_max)
	mvprintw(y++, x, "|");
}

void draw_frame()
{
		int x,y,h;
  y = 2;
  for (x = 5; x < 20; x+=2) {
		  h = (int) ((float)random()/RAND_MAX*20.0);
		  mvprintw(y, x, "%d", h);
		  vertical_bar(y+1, x, h);
  }

}
int main()
{	int ch;
  
		int loop;

		initscr();			/* Start curses mode 		*/
		raw();				/* Line buffering disabled	*/
		keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
		noecho();			/* Don't echo() while we do getch */

		printw("Type any character to see it in bold\n");

		mvprintw(0,0,"A");
		mvprintw(25,0,"B");
		mvprintw(0,80,"C");
		mvprintw(25,80,"D");

		loop = 5;
		while (loop--) {
				draw_frame();
				refresh();
				getch();
		}
		ch = getch();			/* If raw() hadn't been called
								 * we have to press enter before it
								 * gets to the program 		*/
		if(ch == KEY_F(1))		/* Without keypad enabled this will */
				printw("F1 Key pressed");/*  not get to us either	*/
		/* Without noecho() some ugly escape
		 * charachters might have been printed
		 * on screen			*/
		else
		{	printw("The pressed key is ");
				attron(A_BOLD);
				printw("%c", ch);
				attroff(A_BOLD);
		}
		refresh();			/* Print it on to the real screen */
		getch();			/* Wait for user input */
		endwin();			/* End curses mode		  */

		return 0;
}
