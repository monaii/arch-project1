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


void RIT_IRQHandler (void) {
    // Joystick UP
    if((LPC_GPIO1->FIOPIN & (1<<29)) == 0) {
        current_direction = 1;
    }
    // Joystick DOWN
    else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
        current_direction = 2;
    }
    // Joystick LEFT
    else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {
        current_direction = 3;
    }
    // Joystick RIGHT
    else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
        current_direction = 4;
    }
    
    if(game_state == GAME_RUNNING) {
        update_game();
        draw_game();
    }
    
    LPC_RIT->RICTRL |= 0x1;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
