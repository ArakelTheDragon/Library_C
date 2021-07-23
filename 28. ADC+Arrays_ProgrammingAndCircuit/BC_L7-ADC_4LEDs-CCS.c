/************************************************************************
*                                                                       *
*   Filename:      BC_L7-ADC_4LEDs-CCS.c                                *
*   Date:          25/4/12                                              *
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
*   Description:    Lesson 7, example 1                                 *
*                                                                       *
*   Demonstrates basic use of ADC                                       *
*                                                                       *
*   Continuously samples analog input, copying value to 4 x LEDs        *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       AN0     = voltage to be measured (e.g. pot output or LDR)       *
*       RC0-3   = output LEDs (RC3 is MSB)                              *
*                                                                       *
************************************************************************/

#include <16F506.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,INTRC_IO,IOSC4


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure ports
    setup_comparator(NC_NC_NC_NC);  // disable comparators -> RC0, RC1 digital
    setup_vref(FALSE);              // disable CVref -> RC2 usable
    
    // configure ADC
    setup_adc(ADC_CLOCK_INTERNAL);  // clock = INTOSC/4, turn ADC on
    setup_adc_ports(AN0_AN2);       // AN0, AN2 analog
    set_adc_channel(0);             // select channel AN0
                                    // -> AN0 ready for sampling    

    
    //*** Main loop
    while (TRUE)
    {
        // sample and display analog input
        output_c(read_adc() >> 4);  // read ADC and copy high nybble of result to LEDs
    }
}
