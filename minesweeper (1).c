/**
* @file minesweeper.c
* @author Joon-Seok Kim <js@example.com>   retouched by 201324437
* @date 2017/4/25
* @brief simple minesweeper game
*
* @section LICENSE
* This program is free software; you can redistribute it and/or
* ...
* @section DESCRIPTION
*
* ...
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio.h"
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
// game table
unsigned char table_array[MAX][MAX];
// location of cursor
int x = 0, y = 0;
// flag: input mode = 0, flag mode = 1, check mode = 2
int game_mode = 0;
// number of mines

int endDetect = 0;




/**
 *
 *        @brief uncover blank cell
 *        @details *This is a recursive function which uncovers blank cells while they are adjacent
 *        @param int row  target raw
 *        @param int col target col
 *   
 *
 *        @return 1 | success! 
 *        
 *
 */


int main(int argc, char *argv[]) {

	
	int nMines = 10;
	if (argc == 2) {
		nMines = atoi(argv[1]);
	}
	new_game();
        // if endDetect != 0 , no more interpret command
	while (nMines != 0 || endDetect == 0) {			// when nMines becomes 0 you will win the game
		print_table();
		cmdInterpret();
	}
     

     
	return 0;
}
