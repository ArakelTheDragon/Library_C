/************************************************************************
*                                                                       *
*   Filename:      BC_L4-Sleep_LED_off-CCS.c                            *
*   Date:                                                               *
*   File Version:  1.1                                                  *
*                                                                       *
*   Author:        Yordan Itsov                                         *
*   Company:                                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     12F508/509                                           *
*   Compiler:      CCS PCB v5.015                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 4, example 1                                 *
*                                                                       *
*   Demonstrates sleep mode                                             *
*                                                                       *
*   Turn on LED, wait for button pressed, turn off LED, then sleep      *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = indicator LED                                             *
*       GP3 = pushbutton (active low)                                   *
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

// Pin assignments
#define LED     GP1             // Indicator LED
#define BUTTON  GP3             // Pushbutton (active low)


/***** MAIN PROGRAM *****/
void main()
{
    // turn on LED    
    output_high(LED);      

    // wait for button press    
    while (input(BUTTON) == 1)      // wait until button low
        ;

    // go into standby (low power) mode
    output_low(LED);                // turn off LED        
    sleep();                        // enter sleep mode

    while (TRUE)                    // (this loop should never execute)
        ;
}
