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
int maze[MAZE_HEIGHT][MAZE_WIDTH] = {0};
int pills_remaining = 0;
int maze_offset_y = 40;  // Global maze offset
int timer_ticks = 0;  
int VICTORY_SCORE =3000;

void init_game(void) {
    int i, j;
    
    // Initialize maze with walls and pills
    for(i = 0; i < MAZE_HEIGHT; i++) {
        for(j = 0; j < MAZE_WIDTH; j++) {
            if(i == 0 || i == MAZE_HEIGHT-1 || j == 0 || j == MAZE_WIDTH-1) {
                maze[i][j] = WALL;
            } else {
                maze[i][j] = PILL;
            }
        }
    }
// Remove any pills near teleport zones
for(int k = -1; k <= 1; k++) {
    maze[TELEPORT_Y+k][LEFT_EDGE] = WALL;
    maze[TELEPORT_Y+k][RIGHT_EDGE-1] = WALL;
}
// Make teleport zones thicker
maze[TELEPORT_Y][LEFT_EDGE-1] = TELEPORT;
maze[TELEPORT_Y][LEFT_EDGE] = TELEPORT;
maze[TELEPORT_Y][RIGHT_EDGE-1] = TELEPORT;
maze[TELEPORT_Y][RIGHT_EDGE] = TELEPORT;
		
    // Add internal walls
    for(i = 5; i < MAZE_HEIGHT-5; i++) {
        maze[i][10] = WALL;
        maze[i][20] = WALL;
    }
    
    // Add power pills
    int power_pills = 6;
    while(power_pills > 0) {
        i = rand() % (MAZE_HEIGHT-2) + 1;
        j = rand() % (MAZE_WIDTH-2) + 1;
        if(maze[i][j] == PILL) {
            maze[i][j] = POWER_PILL;
            power_pills--;
        }
    }
    
    // Initialize game state
    LCD_Clear(COLOR_BLACK);
    game_state = GAME_PAUSED;
    score = 0;
    lives = 1;
    countdown = 60;
    pacman_x = 120;
    pacman_y = 160;
    
    // Draw welcome screen
    GUI_Text(100, 140, (uint8_t*)"PAC-MAN", COLOR_YELLOW, COLOR_BLACK);
    GUI_Text(70, 160, (uint8_t*)"Press INT0 to Start", COLOR_WHITE, COLOR_BLACK);
}

void update_game(void) {
    if (game_state != GAME_RUNNING) return;

    // Calculate next position
    int next_x = pacman_x;
    int next_y = pacman_y;

    // Update based on current direction
    switch (current_direction) {
        case 1: next_y -= PACMAN_SPEED; break;  // UP
        case 2: next_y += PACMAN_SPEED; break;  // DOWN
        case 3: next_x -= PACMAN_SPEED; break;  // LEFT
        case 4: next_x += PACMAN_SPEED; break;  // RIGHT
        default: return;  // No movement
    }

// Handle teleportation (horizontal only)
    if (next_y / CELL_SIZE == TELEPORT_Y) {
        if (next_x < LEFT_EDGE * CELL_SIZE) {  // Teleport from left to right
            next_x = (RIGHT_EDGE - 1) * CELL_SIZE;
        } else if (next_x >= RIGHT_EDGE * CELL_SIZE) {  // Teleport from right to left
            next_x = LEFT_EDGE * CELL_SIZE;
        }
    }
		
    // Convert to maze grid coordinates
    int maze_x1 = next_x / CELL_SIZE;
    int maze_y1 = next_y / CELL_SIZE;
    int maze_x2 = (next_x + PACMAN_SIZE - 1) / CELL_SIZE;
    int maze_y2 = (next_y + PACMAN_SIZE - 1) / CELL_SIZE;

    // Check collision with walls
    if (maze[maze_y1][maze_x1] == WALL || maze[maze_y1][maze_x2] == WALL ||
        maze[maze_y2][maze_x1] == WALL || maze[maze_y2][maze_x2] == WALL) {
        current_direction = 0;  // Stop movement
        return;
    }



    // Update position
    pacman_x = next_x;
    pacman_y = next_y;

    // Handle pill collection
    if (maze[maze_y1][maze_x1] == PILL) {
        score += 10;
        maze[maze_y1][maze_x1] = EMPTY;
        pills_remaining--;

        // Check for extra life (every 1000 points)
        if (score / 1000 > lives - 1) {
            lives++;
        }
    } else if (maze[maze_y1][maze_x1] == POWER_PILL) {
        score += 50;
        maze[maze_y1][maze_x1] = EMPTY;
        pills_remaining--;

        // Check for extra life
        if (score / 1000 > lives - 1) {
            lives++;
        }
    }

    // Victory condition
 if(pills_remaining == 0) {
        game_state = GAME_WIN;    }
}

void draw_game(int full_redraw) {
	
   if(game_state == GAME_OVER || game_state == GAME_WIN) {
        LCD_Clear(COLOR_BLACK);
        GUI_Text(100, 140, (uint8_t*)(game_state == GAME_OVER ? "GAME OVER!" : "VICTORY!"), 
                game_state == GAME_OVER ? COLOR_RED : COLOR_YELLOW, COLOR_BLACK);
        char score_text[16];
        sprintf(score_text, "Final Score: %d", score);
        GUI_Text(80, 160, (uint8_t*)score_text, COLOR_WHITE, COLOR_BLACK);
        return;
    }
      
    if(full_redraw) {
        LCD_Clear(COLOR_BLACK);
				if(game_state == GAME_PAUSED) {
        GUI_Text(100, 160, (uint8_t*)"PAUSE", COLOR_WHITE, COLOR_BLACK);
    }

        // Draw maze
        int i, j;
        for(i = 0; i < MAZE_HEIGHT; i++) {
            for(j = 0; j < MAZE_WIDTH; j++) {
                switch(maze[i][j]) {
case WALL:
    for(int tx = 0; tx < CELL_SIZE; tx++) {
        for(int ty = 0; ty < CELL_SIZE; ty++) {
            LCD_SetPoint(j*CELL_SIZE + tx, i*CELL_SIZE + ty + maze_offset_y, COLOR_BLUE);
        }
    }
    break;
case TELEPORT:
    for(int px = 0; px < CELL_SIZE; px++) {
        for(int py = 0; py < CELL_SIZE; py++) {
            LCD_SetPoint(j*CELL_SIZE + px, i*CELL_SIZE + py + maze_offset_y, COLOR_RED);
        }
    }
    break;
                    case PILL:
                        LCD_SetPoint(j*CELL_SIZE + CELL_SIZE/2, 
                                   i*CELL_SIZE + CELL_SIZE/2 + maze_offset_y, COLOR_WHITE);
                        break;
                    case POWER_PILL:
                        for(int p = 0; p < 2; p++) {
                            for(int q = 0; q < 2; q++) {
                                LCD_SetPoint(j*CELL_SIZE + CELL_SIZE/2 + p, 
                                           i*CELL_SIZE + CELL_SIZE/2 + q + maze_offset_y, COLOR_WHITE);
                            }
                        }
                        break;
                }
            }
        }
    }
		// Draw score and timer
        char text[16];
        sprintf(text, "Score: %d", score);
        GUI_Text(8, 20, (uint8_t*)text, COLOR_WHITE, COLOR_BLACK);
        sprintf(text, "Time: %d", countdown);
        GUI_Text(160, 20, (uint8_t*)text, COLOR_WHITE, COLOR_BLACK);
        
    
    // Draw Pac-Man (with position tracking)
    static int old_x = -1, old_y = -1;
    if(old_x != -1) {
        for(int i = 0; i < PACMAN_SIZE; i++) {
            for(int j = 0; j < PACMAN_SIZE; j++) {
                LCD_SetPoint(old_x + i, old_y + j + maze_offset_y, COLOR_BLACK);
            }
        }
    }
    
    for(int i = 0; i < PACMAN_SIZE; i++) {
        for(int j = 0; j < PACMAN_SIZE; j++) {
            LCD_SetPoint(pacman_x + i, pacman_y + j + maze_offset_y, COLOR_YELLOW);
        }
    }
    old_x = pacman_x;
    old_y = pacman_y;
    
    // Draw lives
    char lives_text[16];
    sprintf(lives_text, "Lives: %d", lives);
    GUI_Text(8, 290, (uint8_t*)lives_text, COLOR_WHITE, COLOR_BLACK);
		
	
}

