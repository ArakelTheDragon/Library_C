/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp2_2LEDs-CCS.c                              *
*   Date:          14/5/12                                              *
*   File Version:  3.1                                                  *
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
*   Description:    Lesson 6, example 4                                 *
*                                                                       *
*   Demonstrates use of Comparator 2 and programmable voltage reference *
*                                                                       *
*   Turns on Low LED  when C2IN+ < 2.0 V (low light level)              *
*         or High LED when C2IN+ > 3.0 V (high light level)             *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C2IN+ = voltage to be measured (LDR/resistor divider)           *
*       RC3   = "Low" LED                                               *
*       RC1   = "High" LED                                              *
*                                                                       *
************************************************************************/

#include <16F506.h>

#use delay (clock=4000000)  // oscillator frequency for delay_ms()


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,INTRC_IO,IOSC4

// Pin assignments
#define sLO     sPORTC.RC3          // "Low" LED (shadow)
#define sHI     sPORTC.RC1          // "High" LED (shadow)


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
    
    // configure comparators
    setup_comparator(NC_NC|CP2_C0_VREF);    // C1: off
                                            // C2: C2IN+ > CVref
    
    
    //*** Main loop
    while (TRUE)
    {
        // Test for low illumination
        setup_vref(VREF_HIGH | 5);      // CVref = 0.406*Vdd = 2.03 V
        delay_us(10);                   // wait 10 us to settle
        sLO = ~C2OUT;                   // if C2IN+ < CVref turn on Low LED
        
        // Test for high illumination
        setup_vref(VREF_HIGH | 11);     // CVref = 0.594*Vdd = 2.97 V
        delay_us(10);                   // wait 10 us to settle
        sHI = C2OUT;                    // if C2IN+ > CVref turn on High LED
        
        // Display test results
        output_c(sPORTC.port);      // copy shadow to PORTC 
    }
}
