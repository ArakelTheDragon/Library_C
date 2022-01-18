/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp1_Wakeup-CCS.c                             *
*   Date:          17/4/12                                              *
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
*   Description:    Lesson 6, example 2                                 *
*                                                                       *
*   Demonstrates wake-up on comparator change                           *
*                                                                       *
*   Turns on LED for 1 s when comparator 1 output changes,              *
*   then sleeps until the next change                                   *
*   (internal 0.6 V reference with hysteresis)                          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C1IN+ = voltage to be measured (e.g. pot output or LDR)         *
*       C1OUT = comparator output (fed back to input via resistor)      *
*       RC3   - indicator LED                                           *
*                                                                       *
************************************************************************/

#include <16F506.h>

#bit CWUF = 0x03.6          // CWUF flag in STATUS register

#use delay (clock=4000000)  // oscillator frequency for delay_ms()


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,INTRC_IO,IOSC4

// Pin assignments
#define LED     PIN_C3          // indicator LED on RC3


/***** MAIN PROGRAM *****/
void main()
{
    int1    temp;               // temp variable for reading C1
    
    //*** Initialisation
    
    // configure ports
    setup_adc_ports(NO_ANALOGS);            // disable analog inputs -> C1OUT usable
    
    // check for wake-up on comparator change
    if (!CWUF)
    {
        // power-on reset has occurred:
        
        // configure comparators
        setup_comparator(CP1_B0_VREF|       // C1 on: C1IN+ > 0.6V,
                         CP1_OUT_ON_B2|     //        C1OUT pin enabled,
                         CP1_WAKEUP);       //        wake-up on change enabled
                                                    
        // delay 10 ms to allow comparator to settle
        delay_ms(10);
    }
    else
    {
        // wake-up on comparator change occurred:
        
        // flash LED
        
        output_high(LED);       // turn on LED       
        delay_ms(1000);         // delay 1 sec
    }
    
    //*** Sleep until comparator change
    output_low(LED);            // turn off LED
    temp = C1OUT;               // read comparator to clear mismatch condition
    sleep();                    // enter sleep mode
}
