/************************************************************************
*                                                                       *
*   Filename:      BC_L5-Count_7seg_x1-single-CCS.c                     *
*   Date:                                                               *
*   File Version:  1.2                                                  *
*                                                                       *
*   Author:        Yordan Itsov                                         *
*   Company:                                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     16F506                                               *
*   Compiler:      CCS PCB v5.015                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 5, example 1b                                *
*                                                                       *
*   Demonstrates use of lookup tables to drive a 7-segment display      *
*                                                                       *
*   Single digit 7-segment display counts repeating 0 -> 9              *
*   1 second per count, with timing derived from int 4 MHz oscillator   *
*   (single pattern lookup array)                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       RB0-1,RB4, RC1-4 = 7-segment display bus (common cathode)       *
*                                                                       *
************************************************************************/

#include <16F506.h>

#use delay (clock=4000000)      // oscillator frequency for delay_ms()


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,IOSC4,INTRC_IO


/***** LOOKUP TABLES *****/

// pattern table for 7 segment display on ports B and C
const int8 pat7seg[10] = {
    // RC4:1,RB4,RB1:0 = CDBAEFG
    0b1111110,  // 0
    0b1010000,  // 1
    0b0111101,  // 2
    0b1111001,  // 3
    0b1010011,  // 4
    0b1101011,  // 5
    0b1101111,  // 6
    0b1011000,  // 7
    0b1111111,  // 8
    0b1111011   // 9
}; 


/***** MAIN PROGRAM *****/
void main()
{
    unsigned int8   digit;              // digit to be displayed
    
    //*** Initialisation
    // configure ports
    setup_adc_ports(NO_ANALOGS);    // disable all analog and comparator inputs
    setup_comparator(NC_NC_NC_NC);  // -> RB0, RB1, RC0, RC1 digital

        
    //*** Main loop
    while (TRUE)
    {
        // display each digit from 0 to 9 for 1 sec
        for (digit = 0; digit < 10; digit++)
        {
            // display digit by extracting pattern bits for all pins
            output_b((pat7seg[digit] & 1<<2) << 2 |         // RB4
                     (pat7seg[digit] & 0b00000011));        // RB0-1
            output_c((pat7seg[digit] >> 2) & 0b011110);     // RC1-4
        
            // delay 1 sec
            delay_ms(1000);
        }  
    }      
}
