/************************************************************************
*                                                                       *
*   Filename:      BC_L1-Turn_on_LED-12F508-CCS.c                       *
*   Date:          6/2/12                                               *
*   File Version:  1.0                                                  *
*                                                                       *
*   Author:        Yordan Itsov                                         *
*   Company:                                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     12F508                                               *
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

#include <12F508.h>

#define GP0 PIN_B0      // define GP pins
#define GP1 PIN_B1
#define GP2 PIN_B2
#define GP3 PIN_B3
#define GP4 PIN_B4
#define GP5 PIN_B5


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, int RC clock 
#fuses MCLR,NOPROTECT,NOWDT,INTRC


/***** MAIN PROGRAM *****/
void main()
{
    // Initialisation    
    output_b(0b000010);     // configure GPIO as all output and set GP1 high

    // Main loop
    while (TRUE)
    {                       // loop forever
        ;
    }
}

