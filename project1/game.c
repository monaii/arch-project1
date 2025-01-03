#include "game.h"
#include "GLCD/GLCD.h"

// Define global variables
int game_state = GAME_PAUSED;
int score = 0;
int lives = 1;
int countdown = 60;
int pacman_x = 120;
int pacman_y = 160;
int current_direction = 0;

// Add the maze array
int maze[MAZE_HEIGHT][MAZE_WIDTH] = {0};  // Will fill this in init_game
int pills_remaining = 240;

// Initialize game
void init_game(void) {
	    int i, j;
    
    // Initialize maze with walls (1) and pills (2)
    for(i = 0; i < MAZE_HEIGHT; i++) {
        for(j = 0; j < MAZE_WIDTH; j++) {
            // Border walls
            if(i == 0 || i == MAZE_HEIGHT-1 || j == 0 || j == MAZE_WIDTH-1) {
                maze[i][j] = WALL;
            } else {
                maze[i][j] = PILL;  // Fill with pills
            }
        }
    }
    
    // Add some internal walls (you can customize this pattern)
    for(i = 5; i < MAZE_HEIGHT-5; i++) {
        maze[i][10] = WALL;
        maze[i][20] = WALL;
    }
    
    // Add power pills (randomly)
    int power_pills = 6;
    while(power_pills > 0) {
        i = rand() % (MAZE_HEIGHT-2) + 1;
        j = rand() % (MAZE_WIDTH-2) + 1;
        if(maze[i][j] == PILL) {
            maze[i][j] = POWER_PILL;
            power_pills--;
        }
    }
    LCD_Clear(COLOR_BLACK);
    game_state = GAME_PAUSED;
    score = 0;
    lives = 1;
    countdown = 60;
    pacman_x = 120;
    pacman_y = 160;
    
    // Draw initial screen
    GUI_Text(100, 140, (uint8_t*)"PAC-MAN", COLOR_YELLOW, COLOR_BLACK);
    GUI_Text(70, 160, (uint8_t*)"Press INT0 to Start", COLOR_WHITE, COLOR_BLACK);
}

// Update game state
void update_game(void) {
    if(game_state != GAME_RUNNING) return;
    
    // Move Pac-Man based on current_direction
    switch(current_direction) {
        case 1: // UP
            pacman_y -= PACMAN_SPEED;
            break;
        case 2: // DOWN
            pacman_y += PACMAN_SPEED;
            break;
        case 3: // LEFT
            pacman_x -= PACMAN_SPEED;
            break;
        case 4: // RIGHT
            pacman_x += PACMAN_SPEED;
            break;
    }
    
    // Keep Pac-Man in bounds
    if(pacman_x < 0) pacman_x = 0;
    if(pacman_x > 230) pacman_x = 230;
    if(pacman_y < 0) pacman_y = 0;
    if(pacman_y > 280) pacman_y = 280;
}

// Draw game elements
void draw_game(void) {
    int i, j, wx, wy;
    char score_text[16];
    char time_text[16];
    char lives_text[16];
    int maze_offset_y = 40;  // Space for text above maze
    
    // Clear screen first
    LCD_Clear(COLOR_BLACK);
    
    // Draw score and timer before maze
    sprintf(score_text, "Score: %d", score);
    GUI_Text(8, 20, (uint8_t*)score_text, COLOR_WHITE, COLOR_BLACK);
    
    sprintf(time_text, "Time: %d", countdown);
    GUI_Text(160, 20, (uint8_t*)time_text, COLOR_WHITE, COLOR_BLACK);
    
    // Draw maze
    for(i = 0; i < MAZE_HEIGHT; i++) {
        for(j = 0; j < MAZE_WIDTH; j++) {
            switch(maze[i][j]) {
                case WALL:
                    // Draw solid wall
                    for(wx = 0; wx < CELL_SIZE; wx++) {
                        for(wy = 0; wy < CELL_SIZE; wy++) {
                            LCD_SetPoint(j*CELL_SIZE + wx, 
                                       i*CELL_SIZE + wy + maze_offset_y, COLOR_BLUE);
                        }
                    }
                    break;
                    
                case PILL:
                    LCD_SetPoint(j*CELL_SIZE + CELL_SIZE/2, 
                               i*CELL_SIZE + CELL_SIZE/2 + maze_offset_y, COLOR_WHITE);
                    break;
                    
                case POWER_PILL:
                    LCD_SetPoint(j*CELL_SIZE + CELL_SIZE/2, 
                               i*CELL_SIZE + CELL_SIZE/2 + maze_offset_y, COLOR_WHITE);
                    LCD_SetPoint(j*CELL_SIZE + CELL_SIZE/2 + 1, 
                               i*CELL_SIZE + CELL_SIZE/2 + maze_offset_y, COLOR_WHITE);
                    LCD_SetPoint(j*CELL_SIZE + CELL_SIZE/2, 
                               i*CELL_SIZE + CELL_SIZE/2 + 1 + maze_offset_y, COLOR_WHITE);
                    LCD_SetPoint(j*CELL_SIZE + CELL_SIZE/2 + 1, 
                               i*CELL_SIZE + CELL_SIZE/2 + 1 + maze_offset_y, COLOR_WHITE);
                    break;
            }
        }
    }
    
    // Draw Pac-Man
    for(i = 0; i < PACMAN_SIZE; i++) {
        for(j = 0; j < PACMAN_SIZE; j++) {
            LCD_SetPoint(pacman_x + i, pacman_y + j + maze_offset_y, COLOR_YELLOW);
        }
    }
    
    // Draw lives at bottom
    sprintf(lives_text, "Lives: %d", lives);
    GUI_Text(8, 290, (uint8_t*)lives_text, COLOR_WHITE, COLOR_BLACK);
}