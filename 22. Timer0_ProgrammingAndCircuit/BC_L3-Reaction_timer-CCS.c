/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Reaction_timer-CCS.c                           *
*   Date:                                                               *
*   File Version:  1.0                                                  *
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
*   Description:    Lesson 3, example 1                                 *
*                   Reaction Timer game.                                *
*                                                                       *
*   User must attempt to press button within defined reaction time      *
*   after "start" LED lights.  Success is indicated by "success" LED.   *
*                                                                       *
*       Starts with both LEDs unlit.                                    *
*       2 sec delay before lighting "start"                             *
*       Waits up to 1 sec for button press                              *
*       (only) on button press, lights "success"                        *
*       1 sec delay before repeating from start                         *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = success LED                                               *
*       GP2 = start LED                                                 *
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

#use delay (clock=4000000)      // oscillator frequency for delay_ms()

// Pin assignments
#define START   GP2             // LEDs
#define SUCCESS GP1

#define BUTTON  GP3             // pushbutton


/***** CONSTANTS *****/
#define MAXRT   200             // Maximum reaction time (in ms)


/***** MAIN PROGRAM *****/
void main()
{
    unsigned int8   cnt_8ms;            // counter: increments every 8 ms

    // Initialisation
    
    // configure Timer0: timer mode, prescale = 32 (increment every 32 us)
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32);


    // Main loop                                
    while (TRUE)
    {
        // start with both LEDs off
        output_b(0);                    // clear GPIO

        // delay 2 sec
        delay_ms(2000);                 // delay 2000 ms

        // indicate start  
        output_high(START);             // turn on start LED
        
        // wait up to 1 sec for button press
        cnt_8ms = 0;
        while (input(BUTTON) == 1 && cnt_8ms < 1000/8)
        {
            set_timer0(0);                  // clear timer0
            while (get_timer0() < 8000/32)  // wait for 8 ms (32 us/tick)
                ;
            ++cnt_8ms;                      // increment 8 ms counter
        }
        // check elapsed time
        if (cnt_8ms < MAXRT/8)          // if time < max reaction time (8 ms/count)
            output_high(SUCCESS);       //   turn on success LED

        // delay 1 sec
        delay_ms(1000);                 // delay 1000 ms
    
    }   // repeat forever
}
