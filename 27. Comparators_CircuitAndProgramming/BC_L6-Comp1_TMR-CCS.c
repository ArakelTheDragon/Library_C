/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp1_TMR-CCS.c                                *
*   Date:          29/5/12                                              *
*   File Version:  1.2                                                  *
*                                                                       *
*   Author:        David Meiklejohn                                     *
*   Company:       Gooligum Electronics                                 *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     16F506                                               *
*   Compiler:      CCS PCB v4.073                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 6, example 3                                 *
*                                                                       *
*   Demonstrates use of comparator 1 to clock TMR0                      *
*                                                                       *
*   LED flashes at 1 Hz (50% duty cycle),                               *
*   with timing derived from 32.768 kHz input on C1IN+                  *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C1IN+ = 32.768 kHz signal                                       *
*       RC3   = flashing LED                                            *
*                                                                       *
************************************************************************/

#include <16F506.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,INTRC_IO,IOSC4

// Pin assignments
#define sFLASH  sPORTC.RC3          // flashing LED (shadow)


/***** GLOBAL VARIABLES *****/
union {                             // shadow copy of PORTC
    unsigned int8   port;
    struct {
        unsigned    RC0     : 1;
        unsigned    RC1     : 1;
        unsigned    RC2     : 1;
        unsigned    RC3     : 1;
        unsigned    RC4     : 1;
        unsigned    RC5     : 1;
    };
} sPORTC;


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure Timer0 
    setup_timer_0(RTCC_EXT_L_TO_H|RTCC_DIV_128);    // counter mode, prescale = 128
                                                    // -> increment at 256 Hz
                                                    //    with 32.768 kHz input

    // configure comparators
    setup_comparator(CP1_B0_VREF|CP1_TIMER0);       // C1 on: C1IN+ > 0.6 V,
                                                    //        TMR0 clock enabled
    
      
    //*** Main loop
    while (TRUE)
    {
        // TMR0<7> cycles at 1 Hz, so continually copy to LED
        sFLASH = (get_timer0() & 1<<7) != 0;    // sFLASH = TMR0<7>        
        
        output_c(sPORTC.port);                  // copy shadow to PORTC 
    }
}
