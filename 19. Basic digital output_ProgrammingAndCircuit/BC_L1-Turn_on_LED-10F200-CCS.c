/************************************************************************
*                                                                       *
*   Filename:      BC_L1-Turn_on_LED-10F200-CCS.c                       *
*   Date:          19/9/12                                              *
*   File Version:  1.1                                                  *
*                                                                       *
*   Author:        Yordan Itsov                                         *
*   Company:                                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     10F200                                               *
*   Compiler:      CCS PCB v4.073                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 1, example 1                                 *
*                                                                       *
*   Turns on LED.  LED remains on until power is removed.               *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = indicator LED                                             *
*                                                                       *
************************************************************************/

#include <10F200.h>

#define GP0 PIN_B0      // define GP pins
#define GP1 PIN_B1
#define GP2 PIN_B2
#define GP3 PIN_B3


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog
#fuses MCLR,NOPROTECT,NOWDT


/***** MAIN PROGRAM *****/
void main()
{
    // Initialisation    
    output_b(0b0010);       // configure GPIO as all output and set GP1 high

    // Main loop
    while (TRUE)
    {                       // loop forever
        ;
    }
}

