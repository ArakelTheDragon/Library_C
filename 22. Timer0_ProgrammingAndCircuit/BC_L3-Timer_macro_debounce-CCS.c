/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Timer_macro_debounce-CCS.c                     *
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
*   Files required: stdmacros.h     (provides debounce macros)          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 3, example 3b                                *
*                                                                       *
*   Demonstrates use of Timer0-based debounce macros                    *
*                                                                       *
*   Toggles LED when pushbutton is pressed then released                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = flashing LED                                              *
*       GP3 = pushbutton switch (active low)                            *
*                                                                       *
************************************************************************/

#include <12F509.h>

#include "stdmacros-CCS.h"  // DbnceHi() - debounce switch, wait for high
                            // DbnceLo() - debounce switch, wait for low
                            // Requires: TMR0 at 256 us/tick

/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
#fuses NOMCLR,NOPROTECT,NOWDT,INTRC

// Pin assignments
#define sFLASH  sGPIO.GP1       // flashing LED (shadow)
#define BUTTON  PIN_B3          // pushbutton on GP3


/***** GLOBAL VARIABLES *****/
union {                             // shadow copy of GPIO
    unsigned int8   port;
    struct {
        unsigned    GP0     : 1;
        unsigned    GP1     : 1;
        unsigned    GP2     : 1;
        unsigned    GP3     : 1;
        unsigned    GP4     : 1;
        unsigned    GP5     : 1;
    };
} sGPIO;


/***** MAIN PROGRAM *****/
void main()
{
    // Initialisation

    // configure port    
    output_b(0);                        // start with LED off
    sGPIO.port = 0;                     //   update shadow
    
    // configure Timer0: 
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);  // timer mode, prescale = 256
                                                // -> increment every 256 us    
    
    // Main loop
    while (TRUE)
    {
        // wait for button press
        while (input(BUTTON) == 1)          // wait until button low
            ;
        
        // toggle LED
        sFLASH = !sFLASH;                   // toggle flashing LED (shadow)
        output_b(sGPIO.port);               // write to GPIO

        // wait for button release
        DbnceHi(BUTTON);                    // wait until button high (debounced)
            
    }   // repeat forever
}
