#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define SEED 42
#define NUM_TILES_X 9
#define NUM_TILES_Y 9
#define FLAGS 10
#define MINES 10
#define PLAY_MINESWEEPER 1
#define SHOW_LEADERBOARD 2
#define EXIT 3
#define REVEAL_TILE 4
#define PLACE_FLAG 5
#define QUIT_GAME 6
#define MAXSTRINGSIZE 16
#define MAXENTRIES 50

char y_Axis[NUM_TILES_Y] = "ABCDEFGHI";
char x_Axis[NUM_TILES_X] = "123456789";

#endif
