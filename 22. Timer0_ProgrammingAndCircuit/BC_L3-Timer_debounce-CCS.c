/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Timer_debounce-CCS.c                           *
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
*   Description:    Lesson 3, example 3a                                *
*                                                                       *
*   Demonstrates use of Timer0 to implement debounce counting algorithm *
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
    output_b(0);                    // start with LED off
    sGPIO.port = 0;                 //   update shadow
   
    // configure Timer0: 
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_64);   // timer mode, prescale = 64
                                                // -> increment every 64 us

    // Main loop  
    while (TRUE)
    {
        // wait for button press, debounce using timer0:
        set_timer0(0);                          // reset timer
        while (get_timer0() < 157)              // wait at least 10 ms (157 x 64 us = 10 ms)
            if (input(BUTTON) == 1)             //   if button up,
                set_timer0(0);                  //     restart wait
        
        // toggle LED
        sFLASH = !sFLASH;                       // toggle flashing LED (shadow)
        output_b(sGPIO.port);                   // write to GPIO

        // wait until button released (GP3 high), debounce using timer0:
        set_timer0(0);                          // reset timer
        while (get_timer0() < 157)              // wait at least 10 ms (157 x 64 us = 10 ms)
            if (input(BUTTON) == 0)             //   if button down,
                set_timer0(0);                  //     restart wait
  
    }   // repeat forever
}
