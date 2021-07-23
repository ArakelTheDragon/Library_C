/************************************************************************
*                                                                       *
*   Filename:      BC_L1-Flash_LED-20p-CCS.c                            *
*   Date:          6/2/12                                               *
*   File Version:  1.1                                                  *
*                                                                       *
*   Author:        Yordan Itsov                                         *
*   Company:                                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     12F509                                               *
*   Compiler:      CCS PCB v4.073                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 1, example 2                                 *
*                                                                       *
*   Flashes an LED at approx 1 Hz, with 20% duty cycle                  *
*   LED continues to flash until power is removed.                      *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = flashing LED                                              *
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
// ext reset, no code protect, no watchdog, int RC clock 
#fuses MCLR,NOPROTECT,NOWDT,INTRC

#use delay (clock=4000000)      // oscillator frequency for delay_ms()


/***** MAIN PROGRAM *****/
void main()
{
    // Main loop 
    while (TRUE)
    { 
        output_b(0b000010);     // turn on LED on GP1 (bit 1)

        delay_ms(200);          // stay on for 200ms

        output_b(0);            // turn off LED (clearing GPIO clears GP1)

        delay_ms(800);          // stay off for 800ms
        
    }                           // repeat forever
}

