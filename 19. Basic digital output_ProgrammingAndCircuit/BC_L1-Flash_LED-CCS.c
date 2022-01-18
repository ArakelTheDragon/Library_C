/************************************************************************
*                                                                       *
*   Filename:      BC_L1-Flash_LED-CCS.c                                *
*   Date:          8/2/12                                               *
*   File Version:  1.3                                                  *
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
*   Description:    Lesson 1, example 3                                 *
*                                                                       *
*   Flashes an LED at approx 1 Hz.                                      *
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


/***** GLOBAL VARIABLES *****/
unsigned int8   sGPIO = 0;      // shadow copy of GPIO


/***** MAIN PROGRAM *****/
void main()
{
   // Main loop 
   while (TRUE)
   { 
        // toggle LED on GP1       
        sGPIO ^= 0b000010;      // toggle shadow bit corresponding to GP1
        output_b(sGPIO);        // write to GPIO 

        // delay 500ms
        delay_ms(500); 
                
   }    // repeat forever
}

