/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp1_LED-neg-CCS.c                            *
*   Date:          12/4/12                                              *
*   File Version:  1.1                                                  *
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
*   Description:    Lesson 6, example 1b                                *
*                                                                       *
*   Demonstrates basic use of Comparator 1 polarity bit                 *
*                                                                       *
*   Turns on LED when voltage on C1IN+ < voltage on C1IN-               *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C1IN+ = voltage to be measured (e.g. pot output or LDR)         *
*       C1IN- = threshold voltage (set by voltage divider resistors)    *
*       RC3   = indicator LED                                           *
*                                                                       *
************************************************************************/

#include <16F506.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,IOSC4,INTRC_IO

// Pin assignments
#define LED     PIN_C3          // indicator LED on RC3


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure comparators
    setup_comparator(CP1_B0_B1|CP1_INVERT); // C1 on, C1OUT = 1 if C1IN+ < C1IN-

    
    //*** Main loop
    while (TRUE)
    {
        // continually display comparator output
        output_bit(LED,C1OUT);
    }
}
