/************************************************************************
*                                                                       *
*   Filename:      BC_L2-PB_LED-CCS.c                                   *
*   Date:                                                               *
*   File Version:  1.1                                                  *
*                                                                       *
*   Author:        Yordan Itsov                                         *
*   Company:                                        					*
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     12F509                                               *
*   Compiler:      CCS PCB v5.015                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 2, example 1                                 *
*                                                                       *
*   Demonstrates reading a switch                                       *
*                                                                       *
*   Turns on LED when pushbutton is pressed                             *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = indicator LED                                             *
*       GP3 = pushbutton switch (active low)                            *
*                                                                       *
************************************************************************/

#include <12F509.h>

#define GP0 PIN_B0              // define GP pins
#define GP1 PIN_B1
#define GP2 PIN_B2
#define GP3 PIN_B3
#define GP4 PIN_B4
#define GP5 PIN_B5

/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
#fuses NOMCLR,NOPROTECT,NOWDT,INTRC


/***** MAIN PROGRAM *****/
void main()
{
    // Main loop
    while (TRUE)
    {
        // turn on LED only if button pressed
        output_b(input(GP3) ? 0 : 0b000010);    // if GP3 high, clear GP1 
                                                //   else set GP1
    }   // repeat forever
}
