/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp1_LED-intref-CCS.c                         *
*   Date:          17/4/12                                              *
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
*   Description:    Lesson 6, example 1c                                *
*                                                                       *
*   Demonstrates use of Comparator 1 internal 0.6 V reference           *
*                                                                       *
*   Turns on LED when voltage on C1IN+ < 0.6 V                          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C1IN+ = voltage to be measured (e.g. pot output or LDR)         *
*       RC3   = indicator LED                                           *
*                                                                       *
************************************************************************/

#include <16F506.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,INTRC_IO,IOSC4

// Pin assignments
#define LED     PIN_C3          // indicator LED on RC3


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure comparators
    setup_comparator(CP1_B0_VREF|CP1_INVERT);   // C1 on: C1IN+ < 0.6 V

    
    //*** Main loop
    while (TRUE)
    {
        // continually display comparator output
        output_bit(LED,C1OUT);  
    }
}
