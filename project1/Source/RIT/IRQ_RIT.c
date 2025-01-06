/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../game.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


// In RIT_IRQHandler
void RIT_IRQHandler (void) {
    // Timer logic only when game is running
    if(game_state == GAME_RUNNING) {
        timer_ticks++;
        if(timer_ticks >= 2) {
            timer_ticks = 0;
            if(countdown > 0) {
                countdown--;
                if(countdown == 0) {
                    game_state = GAME_OVER;
                }
            }
        }
        
        // Joystick handling and updates only when not paused
        if((LPC_GPIO1->FIOPIN & (1<<29)) == 0) {
            current_direction = 1;
        }
        else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
            current_direction = 2;
        }
        else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {
            current_direction = 3;
        }
        else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
            current_direction = 4;
        }
        
        update_game();
        draw_game(0);
    }
    
    LPC_RIT->RICTRL |= 0x1;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
