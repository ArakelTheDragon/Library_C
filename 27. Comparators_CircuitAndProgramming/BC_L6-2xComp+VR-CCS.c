/************************************************************************
*                                                                       *
*   Filename:      BC_L6-2xComp2+VR-CCS.c                               *
*   Date:          14/5/12                                              *
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
*   Description:    Lesson 6, example 5                                 *
*                                                                       *
*   Demonstrates use of comparators 1 and 2                             *
*   with the programmable voltage reference                             *
*                                                                       *
*   Turns on: LED 1 when C1IN+ > 2.5 V                                  *
*         and LED 2 when C2IN+ > 2.5 V                                  *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C1IN+ = input 1 (LDR/resistor divider)                          *
*       C1IN- = connected to CVref                                      *
*       C2IN+ = input 2 (LDR/resistor divider)                          *
*       CVref = connected to C1IN-                                      *
*       RC1   = indicator LED 2                                         *
*       RC3   = indicator LED 1                                         *
*                                                                       *
************************************************************************/

#include <16F506.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,INTRC_IO,IOSC4

// Pin assignments
#define sLED1   sPORTC.RC3      // indicator LED 1
#define sLED2   sPORTC.RC1      // indicator LED 2


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
    setup_comparator(CP1_B0_B1|CP2_C0_VREF);    // C1 on: C1IN+ > C1IN-
                                                // C2 on: C2IN+ > CVref
    // configure voltage reference    
    setup_vref(VREF_LOW | 12 | VREF_A2);        // CVref = 0.500*Vdd = 2.50 V,
                                                // CVref output pin enabled
    
    //*** Main loop
    while (TRUE)
    {
        // start with shadow PORTC clear
        sPORTC.port = 0;
        
        // test comparator inputs
        sLED1 = C1OUT;              // turn on LED 1 if C1IN+ > CVref
        sLED2 = C2OUT;              // turn on LED 2 if C2IN+ > CVref
        
        // display test results
        output_c(sPORTC.port);      // copy shadow to PORTC
    }
}
