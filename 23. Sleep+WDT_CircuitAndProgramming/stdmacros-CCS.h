/************************************************************************
*                                                                       *
*   Filename:      stdmacros-CCS.h                                      *
*   Date:                                                               *
*   File Version:  1.1                                                  *
*                                                                       *
*   Author:        Yordan Itsov                                         *
*   Company:                                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline or Midrange PIC                             *
*   Processor:     any                                                  *
*   Compiler:      CCS PCB or PCM                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Library of useful macros                            *
*                                                                       *
*   DbnceHi - debounce switch, wait for high input                      *
*   DbnceLo - debounce switch, wait for low input                       *
*                                                                       *
************************************************************************/


/***** DEBOUNCE MACROS *****/

#define DEBOUNCE 10*1000/256    // switch debounce count = 10 ms/(256us/tick)

// DbnceHi()
//
// Debounce switch on given input pin
// Waits for switch input to be high continuously for DEBOUNCE*256/1000 ms
//
// Uses: TMR0       Assumes: TMR0 running at 256 us/tick
//
#define DbnceHi(PIN)                                                       \
    set_timer0(0);                          /* reset timer              */ \
    while (get_timer0() < DEBOUNCE)         /* wait until debounce time */ \
        if (input(PIN) == 0)                /*   if input low,          */ \
            set_timer0(0)                   /*     restart wait         */
                        

// DbnceLo()
//
// Debounce switch on given input pin
// Waits for switch input to be low continuously for 10 ms
//
// Uses: TMR0       Assumes: TMR0 running at 256 us/tick
//
#define DbnceLo(PIN)                                                       \ 
    set_timer0(0);                          /* reset timer              */ \
    while (get_timer0() < DEBOUNCE)         /* wait until debounce time */ \
        if (input(PIN) == 1)                /*   if input high,         */ \
            set_timer0(0)                   /*     restart wait         */

