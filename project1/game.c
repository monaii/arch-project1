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
// Update game state
void update_game(void) {
    if (game_state != GAME_RUNNING) return; // Only update if the game is running
	  char dir_debug[32];
    sprintf(dir_debug, "Direction: %d", current_direction);
    GUI_Text(10, 340, (uint8_t*)dir_debug, COLOR_WHITE, COLOR_BLACK);
	
	
    // Temporary variables for the next position
    int next_x = pacman_x;
    int next_y = pacman_y;
		char position_debug[32];
		sprintf(position_debug, "X: %d, Y: %d", pacman_x, pacman_y);
		GUI_Text(10, 300, (uint8_t*)position_debug, COLOR_WHITE, COLOR_BLACK);


    // Calculate the next position based on the current direction
    switch (current_direction) {
        case 1: // UP
            next_y -= PACMAN_SPEED;
            break;
        case 2: // DOWN
            next_y += PACMAN_SPEED;
            break;
        case 3: // LEFT
            next_x -= PACMAN_SPEED;
            break;
        case 4: // RIGHT
            next_x += PACMAN_SPEED;
            break;
        default:
            return; // No movement if no direction is set
    }
		sprintf(position_debug, "Next X: %d, Next Y: %d", next_x, next_y);
    GUI_Text(10, 320, (uint8_t*)position_debug, COLOR_WHITE, COLOR_BLACK);

    // Convert the next position to maze grid coordinates
    int maze_x = next_x / CELL_SIZE;
    int maze_y = next_y / CELL_SIZE;

    // Check if the next position is within bounds and not a wall
    if (maze_x >= 0 && maze_x < MAZE_WIDTH &&
        maze_y >= 0 && maze_y < MAZE_HEIGHT &&
        maze[maze_y][maze_x] != WALL) {
        // Update Pac-Man's position if the move is valid
        pacman_x = next_x;
        pacman_y = next_y;
    } else {
        // Stop movement if the next position is a wall
        current_direction = 0;
    }
}

// Draw game elements
void draw_game(int full_redraw) {
    int i, j, wx, wy;
    char score_text[16];
    char time_text[16];
    char lives_text[16];
    int maze_offset_y = 40;  // Space for text above maze
	
    if(full_redraw) {
			
        // Only clear and redraw everything on full redraw
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
	}
   // Always draw Pac-Man (erase old position first)
    static int old_x = -1, old_y = -1;
    if(old_x != -1) {
        // Erase old position
        for(i = 0; i < PACMAN_SIZE; i++) {
            for(j = 0; j < PACMAN_SIZE; j++) {
                LCD_SetPoint(old_x + i, old_y + j + maze_offset_y, COLOR_BLACK);
            }
        }
    }
		  // Draw new position
    for(i = 0; i < PACMAN_SIZE; i++) {
        for(j = 0; j < PACMAN_SIZE; j++) {
            LCD_SetPoint(pacman_x + i, pacman_y + j + maze_offset_y, COLOR_YELLOW);
        }
    }
    old_x = pacman_x;
    old_y = pacman_y;

    
    // Draw lives at bottom
    sprintf(lives_text, "Lives: %d", lives);
    GUI_Text(8, 290, (uint8_t*)lives_text, COLOR_WHITE, COLOR_BLACK);
}