/************************************************************************
*                                                                       *
*   Filename:      BC_L2-Toggle_LED+WPU-CCS.c                           *
*   Date:                                                               *
*   File Version:  1.1                                                  *
*                                                                       *
*   Author:        Yordan Itsov                                         *
*   Company:                                                            *
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
*   Description:    Lesson 2, example 3                                 *
*                                                                       *
*   Demonstrates use of weak pull-ups                                   *
*                                                                       *
*   Toggles LED when pushbutton is pressed then released,               *
*   Uses counting algorithm to debounce switch                          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = indicator LED                                             *
*       GP3 = pushbutton switch                                         *
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

#use delay (clock=4000000)      // oscillator frequency for delay_ms()


/***** GLOBAL VARIABLES *****/
unsigned int8   sGPIO = 0;          // shadow copy of GPIO


/***** MAIN PROGRAM *****/
void main()
{
    unsigned int8   db_cnt;         // debounce counter

    // Initialisation
    setup_counters(RTCC_INTERNAL,RTCC_DIV_1);   // enable weak pull-ups
    output_b(0);                    // start with LED off
    sGPIO = 0;                      //   update shadow

    // Main loop
    while (TRUE)
    {
        // wait for button press, debounce by counting:
        for (db_cnt = 0; db_cnt <= 10; db_cnt++)
        {
            delay_ms(1);            // sample every 1 ms
            if (input(GP3) == 1)    // if button up (GP3 high)
                db_cnt = 0;         //   restart count
        }                           // until button down for 10 successive reads

        // toggle LED on GP1
        sGPIO ^= 0b000010;          // toggle shadow GP1
        output_b(sGPIO);            // write to GPIO

        // wait for button release, debounce by counting:
        for (db_cnt = 0; db_cnt <= 10; db_cnt++)
        {
            delay_ms(1);            // sample every 1 ms
            if (input(GP3) == 0)    // if button down (GP3 low)
                db_cnt = 0;         //   restart count
        }                           // until button up for 10 successive reads

    }   // repeat forever
}
