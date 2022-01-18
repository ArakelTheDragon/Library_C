/************************************************************************
*                                                                       *
*   Filename:      BC_L5-Count_7seg_x1-CCS.c                            *
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
*   Description:    Lesson 5, example 1a                                *
*                                                                       *
*   Demonstrates use of lookup tables to drive a 7-segment display      *
*                                                                       *
*   Single digit 7-segment display counts repeating 0 -> 9              *
*   1 second per count, with timing derived from int 4 MHz oscillator   *
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

// pattern table for 7 segment display on port B
const int8 pat7segB[10] = {
    // RB4 = E, RB1:0 = FG
    0b010010,   // 0
    0b000000,   // 1
    0b010001,   // 2
    0b000001,   // 3
    0b000011,   // 4
    0b000011,   // 5
    0b010011,   // 6
    0b000000,   // 7
    0b010011,   // 8
    0b000011    // 9    
}; 

// pattern table for 7 segment display on port C
const int8 pat7segC[10] = {
    // RC4:1 = CDBA
    0b011110,   // 0
    0b010100,   // 1
    0b001110,   // 2
    0b011110,   // 3
    0b010100,   // 4
    0b011010,   // 5
    0b011010,   // 6
    0b010110,   // 7
    0b011110,   // 8
    0b011110    // 9
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
            // display digit
            output_b(pat7segB[digit]);    // lookup port B and C patterns
            output_c(pat7segC[digit]);
        
            // delay 1 sec
            delay_ms(1000);
        }  
    }      
}
