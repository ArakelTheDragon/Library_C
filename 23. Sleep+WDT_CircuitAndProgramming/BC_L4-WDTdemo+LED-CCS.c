/************************************************************************
*                                                                       *
*   Filename:      BC_L4-WDTdemo+LED-CCS.c                              *
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
*   Description:    Lesson 4, example 3a                                *
*                                                                       *
*   Demonstrates watchdog timer                                         *
*       plus differentiation from POR reset                             *
*                                                                       *
*   Turn on LED for 1 s, turn off, then enter endless loop              *
*   If enabled, WDT timer restarts after 2.3 s -> LED flashes           *
*   Turns on WDT LED to indicate WDT reset                              *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = flashing LED                                              *
*       GP2 = WDT-reset indicator LED                                   *
*                                                                       *
************************************************************************/

#include <12F509.h>

#define GP0 PIN_B0              // define GP pins
#define GP1 PIN_B1
#define GP2 PIN_B2
#define GP3 PIN_B3
#define GP4 PIN_B4
#define GP5 PIN_B5

#use delay (clock=4000000)      // oscillator frequency for delay_ms()


/***** CONFIGURATION *****/
#define     WATCHDOG            // define to enable watchdog timer

#ifdef WATCHDOG
    // ext reset, no code protect, watchdog, int RC clock
    #fuses MCLR,NOPROTECT,WDT,INTRC
#else
     // ext reset, no code protect, no watchdog, int RC clock
    #fuses MCLR,NOPROTECT,NOWDT,INTRC
#endif   

// Pin assignments
#define LED     GP1             // LED to flash
#define WDT     GP2             // watchdog timer reset indicator


/***** MAIN PROGRAM *****/
void main()
{
    //***** Initialisation
    // configure port
    output_b(0);                            // start with both LEDs off
    
    // test for WDT-timeout reset
    // (note: must be done before initialising watchdog timer)
    if (restart_cause() == WDT_TIMEOUT)     // if WDT timeout has occurred,
        output_high(WDT);                   //   turn on "error" LED

    // configure watchdog timer      
    setup_wdt(WDT_2304MS);                  // WDT period = 2.3 s 

    
    //***** Main code 
    // flash LED
    output_high(LED);                       // turn on "flash" LED
    delay_ms(1000);                         // delay 1 sec
    output_low(LED);                        // turn off "flash" LED 

    // wait forever    
    while (TRUE)                            
        ;
}
