#include "button.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "../game.h"


// In IRQ_button.c
void EINT0_IRQHandler (void) {
    if(game_state == GAME_RUNNING) {
        game_state = GAME_PAUSED;
        GUI_Text(100, 160, (uint8_t*)"PAUSE", COLOR_WHITE, COLOR_BLACK);
    }
    else if(game_state == GAME_PAUSED) {
        game_state = GAME_RUNNING;
        draw_game(1);  // Full redraw to clear pause message
    }
    
    LPC_SC->EXTINT &= (1 << 0);
}
