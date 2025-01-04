#ifndef __GAME_H
#define __GAME_H

#include "LPC17xx.h"
#include <stdio.h>

// Game states
#define GAME_PAUSED 0
#define GAME_RUNNING 1
#define GAME_OVER 2
#define GAME_WIN 3

// Colors
#define COLOR_BLACK 0x0000
#define COLOR_BLUE 0x001F
#define COLOR_YELLOW 0xFFE0
#define COLOR_WHITE 0xFFFF
#define COLOR_RED 0xF800

// Pac-Man settings
#define PACMAN_SPEED 6
#define PACMAN_SIZE 8


// Maze settings
#define MAZE_WIDTH 30
#define MAZE_HEIGHT 25
#define CELL_SIZE 8

// Pill types
#define EMPTY 0
#define WALL 1
#define PILL 2
#define POWER_PILL 3

// Game variables
extern int game_state;
extern int score;
extern int lives;
extern int countdown;
extern int pacman_x;
extern int pacman_y;
extern int current_direction;

// Function declarations
void init_game(void);
void update_game(void);
void draw_game(int full_redraw);

// Additional game variables
extern int maze[MAZE_HEIGHT][MAZE_WIDTH];
extern int pills_remaining;

#endif