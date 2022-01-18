/************************************************************************
*                                                                       *
*   Filename:      BC_L4-Wakeup+LED-CCS.c                               *
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
*   Files required: stdmacros-CCS.h     (provides DbnceHi macro)        *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 4, example 2                                 *
*                                                                       *
*   Demonstrates wake-up on change                                      *
*       plus differentiation from POR reset                             *
*                                                                       *
*   Turn on LED after each reset                                        *
*   Turn on WAKE LED only if reset was due to wake on change            *
*   then wait for button press, turn off LEDs, debounce, then sleep     *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = on/off indicator LED                                      *
*       GP2 = wake-on-change indicator LED                              *
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

#bit GPWUF = 0x03.7         // GPWUF flag in STATUS register

#include "stdmacros-CCS.h"  // DbnceHi() - debounce switch, wait for high
                            // Requires: TMR0 at 256us/tick

/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock
#fuses NOMCLR,NOPROTECT,NOWDT,INTRC

// Pin assignments
#define LED     GP1             // LED to turn on/off
#define WAKE    GP2             // indicates wake on change condition
#define BUTTON  GP3             // Pushbutton (active low)


/***** MAIN PROGRAM *****/
void main()
{
    //***** Initialisation
    // configure port
    output_b(0);                // start with both LEDs off

    // configure wake-on-change and timer    
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);  // enable wake-up on change
                                                // configure Timer0:
                                                //  timer mode, prescale = 256 
                                                //  -> increment every 256 us

    //***** Main code  
    // turn on LED
    output_high(LED);           // turn on LED

    // test for wake-on-change reset        
    if (GPWUF)                  // if wake on change has occurred, 
    {                
        output_high(WAKE);      //   turn on wake-up indicator
        DbnceHi(BUTTON);        //   wait for button to stop bouncing
    }

    // wait for button press    
    while (input(BUTTON) == 1)  // wait until button low
        ;

    // go into standby (low power) mode
    output_b(0);                // turn off both LEDs
    
    DbnceHi(BUTTON);            // wait for stable button release
        
    sleep();                    // enter sleep mode
}
