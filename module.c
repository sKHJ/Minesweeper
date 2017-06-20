#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "module.h"
#define MAX 10

// background color
#define KNRM  "\x1B[0m"
#define BRED  "\x1B[41m"
#define BGRN  "\x1B[42m"
#define BYEL  "\x1B[43m"
#define BBLU  "\x1B[44m"
#define BMAG  "\x1B[45m"
#define BCYN  "\x1B[46m"
#define BWHT  "\x1B[47m"
// text color
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


// global variables

// location of cursor
extern int x, y;
// flag: input mode = 0, flag mode = 1, check mode = 2
extern int game_mode;
// number of mines
int nMines=0;

extern int endDetect;
// game table
unsigned char table_array[MAX][MAX];

int uncover_blank_cell(int row, int col) {
	int value, rows[8], columns[8], i;

	if (table_array[row][col] != 0)
		return 0; // error

	table_array[row][col] += 10; // uncover current cell

								 // Get position of adjacent cells of current cell
	rows[0] = row - 1;
	columns[0] = col + 1;
	rows[1] = row;
	columns[1] = col + 1;
	rows[2] = row + 1;
	columns[2] = col + 1;
	rows[3] = row - 1;
	columns[3] = col;
	rows[4] = row + 1;
	columns[4] = col;
	rows[5] = row - 1;
	columns[5] = col - 1;
	rows[6] = row;
	columns[6] = col - 1;
	rows[7] = row + 1;
	columns[7] = col - 1;

	for (i = 0; i < 8; i++) {
		value = table_array[rows[i]][columns[i]];

		if ((rows[i] >= 0 && rows[i] < MAX) && (columns[i] >= 0 && columns[i] < MAX)) {	// to prevent negative index and out of bounds
			if (value > 0 && value <= 8)
				table_array[rows[i]][columns[i]] += 10;										// it is a cell with 1-8 number so we need to uncover
			else if (value == 0)
				uncover_blank_cell(rows[i], columns[i]);
		}

	}

	return 1; // success!
}
/**
*
*        @brief print_table
*        @details print game board & interface

*
*
*/
void print_table() {
	// clear screen
	system("clear");

	int i, j, value;
	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++) {
			if (x == j && y == i) {
				if (game_mode == 1) {
					printf("|%sF%s", BMAG, KNRM);
					continue;
				}
				else if (game_mode == 2) {
					printf("|%sC%s", BMAG, KNRM);
					continue;
				}

			}
			value = table_array[i][j];

			if ((value >= 0 && value <= 8) || value == 0 || value == 99)
				printf("|X");
			else if (value == 10) // clean area
				printf("|%s%d%s", KCYN, value - 10, KNRM);
			else if (value == 11) // the number of near mine is 1
				printf("|%s%d%s", KYEL, value - 10, KNRM);
			else if (value > 11 && value <= 18) // the number of near mine is greater than 1
				printf("|%s%d%s", KRED, value - 10, KNRM);
			else if ((value >= 20 && value <= 28) || value == 100)
				printf("|%sF%s", KGRN, KNRM);
			else
				printf("ERROR"); // test purposes

		}
		printf("|\n");
	}

	printf("cell values: 'X' unknown, '%s0%s' no mines close, '1-8' number of near mines, '%sF%s' flag in cell\n", KCYN, KNRM, KGRN, KNRM);
	if (game_mode == 0) {
		printf("f (put/remove Flag in cell), c (Check cell), n (New game), q (Exit game): ");
	}
	else if (game_mode == 1) {
		printf("Enter (select to put/remove Flag in cell), q (Exit selection): ");
	}
	else if (game_mode == 2) {
		printf("Enter (select to check cell), q (Exit selection): ");
	}


}
/**
*
*        @brief new_game
*        @details Randomly put mine

*
*
*/
void new_game() {
	int i, j, r, c, value, rows[8], columns[8];

	srand(time(NULL));						// random seed
											// setting cursor
	x = 0;
	y = 0;
	// set all cells to 0
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			table_array[i][j] = 0;

	for (i = 0; i < nMines; i++) {
		/* initialize random seed: */

		r = rand() % 10;					// it generates a integer in the range 0 to 9
		c = rand() % 10;

		// put mines
		if (table_array[r][c] != 99) {
			table_array[r][c] = 99;

			// Get position of adjacent cells of current cell
			rows[0] = r - 1;
			columns[0] = c + 1;
			rows[1] = r;
			columns[1] = c + 1;
			rows[2] = r + 1;
			columns[2] = c + 1;
			rows[3] = r - 1;
			columns[3] = c;
			rows[4] = r + 1;
			columns[4] = c;
			rows[5] = r - 1;
			columns[5] = c - 1;
			rows[6] = r;
			columns[6] = c - 1;
			rows[7] = r + 1;
			columns[7] = c - 1;

			for (j = 0; j < 8; j++) {
				value = table_array[rows[j]][columns[j]];
				if ((rows[j] >= 0 && rows[j] < MAX) && (columns[j] >= 0 && columns[j] < MAX)) {	// to prevent negative index and out of bounds
					if (value != 99)																// to prevent remove mines
						table_array[rows[j]][columns[j]] += 1;									// sums 1 to each adjacent cell
				}
			}

		}
		else {							// to make sure that there are the properly number of mines in table
			i--;
			continue;
		}
	}
}
/**
*
*        @brief check_mode
*        @details check that cell is blank or mine

*
*
*
*
*/
void check_mode() {  // check cell
	int value;
	char direction;
	game_mode = 2;
	do {
		print_table();
		direction = _getch();

		// arrow direction
		if (direction == '8') {
			// up
			y = (MAX + --y) % MAX;
		}
		else if (direction == '2') {
			// down
			y = ++y % MAX;
		}
		else if (direction == '4') {
			x = (MAX + --x) % MAX;
		}
		else if (direction == '6') {
			x = ++x % MAX;
		}
		else if (direction == 'f' || direction == 'F') {
			flag_mode();
		}

		else if (direction == '\n') {
			value = table_array[y][x];
			if (value == 0)						// blank case
				uncover_blank_cell(y, x);
			else if (value == 99)				// mine case
				end_of_game();
			else if (value > 0 && value <= 8)	// number case (the next cell is a mine)
				table_array[y][x] += 10;

			//	break;
		}
	} while (direction != 'q' && direction != 'Q');
	game_mode = 0;
}
/**
*
*        @brief flag mode
*        @details set up a flag on cell that is in doubt

*
* *
*
*/
void flag_mode() {   // flag
	int value;
	char direction;
	game_mode = 1;
	do {
		print_table();
		direction = _getch();
		// arrow direction
		if (direction == '8') {
			// up
			y = (MAX + --y) % MAX;
		}
		else if (direction == '2') {
			// down
			y = ++y % MAX;
		}
		else if (direction == '4') {
			x = (MAX + --x) % MAX;
		}
		else if (direction == '6') {
			x = ++x % MAX;
		}
		else if (direction == 'c' || direction == 'C') {
			check_mode();
		}
		else if (direction == '\n') {
			value = table_array[y][x];

			if (value == 99) {				// mine case
				table_array[y][x] += 1;
				nMines -= 1;				// mine found
			}
			else if (value >= 0 && value <= 8) {	// number of mines case (the next cell is a mine)
				table_array[y][x] += 20;
			}
			else if (value >= 20 && value <= 28) {
				table_array[y][x] -= 20;
			}

			if (nMines == 0)
				break;
		}
	} while (direction != 'q' && direction != 'Q');
	game_mode = 0;
}
/**
*
*        @brief end of game
*        @details end the game and print message


*
*
*/
void end_of_game() {
	char ch;
	game_mode = 0;
	print_table();
	printf("\nGAME OVER\n");

	if (nMines == 0)
		printf("you won!!!!\n");

	else
		printf("BOOM! you LOOSE!\n");

	do {
		printf("Are you sure to exit? (y or n)? ");
		ch = _getch();
		putchar('\n');
		if (ch == 'y' || ch == 'Y') {
			exit(0); break;
		}
		else if (ch == 'n' || ch == 'N') {
			new_game(); break;
		}
		printf("Please answer y or n\n");
	} while (1);
	endDetect = 1;
	printf("See you next time!\n");

	fflush(stdin);
}

/**
*
*        @brief command interpreter
*        @details interprete command and excute it
*        @param int row  target raw

*
*
*/
void cmdInterpret() {
	char ch = getch();
	// cursor direction
	char direction;


	switch (ch) {
	case 'f':
		flag_mode(); break;
	case 'F':
		flag_mode(); break;
	case 'c':
		check_mode();  break;
	case 'C':
		check_mode();  break;
	case 'n':
		new_game(); break;
	case 'N':
		new_game(); break;
	case 'q':
		end_of_game(); break;
	case 'Q':
		end_of_game(); break;
	default:
		break;
	}

}