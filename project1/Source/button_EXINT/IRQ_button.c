#include "button.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "../game.h"


void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	 if(game_state == GAME_PAUSED) {
        game_state = GAME_RUNNING;
        draw_game(1);  // Force redraw when starting
    }
    
    LPC_SC->EXTINT &= (1 << 0);     // clear pending interrupt
	}


