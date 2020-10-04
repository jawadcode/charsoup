#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#define DELAY 40000

typedef struct vector
{
	int x;
	int y;
} vector;

typedef struct letter
{
	char val[2];
	vector pos;
	vector vel;
	short colour;
} letter;

char *strfromchar(char dest[], char src) 
{
	dest[0] = src;
	dest[1] = '\0';
	return dest;
} 

int usage()
{
	printf("Incorrect usage of charactersoup\n");
	printf("Arguments:\n");
	printf("  -n, --number [number of characters]\n");
	return 0;
}

void populate(int maxy, int maxx, int numofletters, letter letters[])
{
	char chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-=!$&*()_+[]{};'#:@~,./<>?";
	for (int i = 0; i < numofletters; i++)
	{
		letters[i].val[0] = chars[rand() % 89];
		letters[i].val[1] = '\0';
		letters[i].pos.x = (rand() % (maxx - 1)) + 1;
		letters[i].pos.y = (rand() % (maxy - 1)) + 1;
		letters[i].vel.x = (rand() % 4) + 1;
		letters[i].vel.y = (rand() % 4) + 1;
		letters[i].colour = (rand() % 7) + 1;
	}
}

void update(int maxy, int maxx, int numofletters, letter letters[])
{
	clear();
	for (int i = 0; i < numofletters; i++)
	{
		attron(COLOR_PAIR(letters[i].colour));
		mvprintw(letters[i].pos.y, letters[i].pos.x, letters[i].val);
		if (letters[i].pos.x >= maxx || letters[i].pos.x < 1) letters[i].vel.x = -letters[i].vel.x;
		if (letters[i].pos.y >= maxy || letters[i].pos.y < 1) letters[i].vel.y = -letters[i].vel.y;
		letters[i].pos.x += letters[i].vel.x;
		letters[i].pos.y += letters[i].vel.y;
	}
	refresh();
	usleep(DELAY);
}

int main(int argc, char const *argv[])
{
	int numofchars;
	if (argc >= 3)
	{
		if (strcmp(argv[1], "-n") || strcmp(argv[1], "--number")) 
		{
			numofchars = atoi(argv[2]);
			if (numofchars < 1) exit(usage());
		} else exit(usage());
	} else exit(usage());

	vector scrsize;
	letter letters[numofchars];
	WINDOW *win = initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	noecho();
	curs_set(FALSE);
	srand(time(NULL));
	getmaxyx(win, scrsize.y, scrsize.x);
	populate(scrsize.y, scrsize.x, numofchars, letters);
	while (1) {
		update(scrsize.y, scrsize.x, numofchars, letters);
	}
	endwin();
}