/************************************************************************
*                                                                       *
*   Filename:      BC_L5-Count_7seg_x3-CCS.c                            *
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
*   Description:    Lesson 5, example 2                                 *
*                                                                       *
*   Demonstrates use of multiplexing to drive multiple 7-seg displays   *
*                                                                       *
*   3-digit 7-segment LED display: 1 digit minutes, 2 digit seconds     *
*   counts in seconds 0:00 to 9:59 then repeats,                        *
*   with timing derived from int 4 MHz oscillator                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       RB0-1,RB4,RC1-4 = 7-segment display bus (common cathode)        *
*       RC5             = minutes enable (active high)                  *
*       RB5             = tens enable                                   *
*       RC0             = ones enable                                   *
*                                                                       *
************************************************************************/

#include <16F506.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,IOSC4,INTRC_IO

// Pin assignments
#define MINUTES PIN_C5              // minutes enable
#define TENS    PIN_B5              // tens enable
#define ONES    PIN_C0              // ones enable


/***** PROTOTYPES *****/
void set7seg(unsigned int8 digit);      // display digit on 7-segment display


/***** MACROS *****/
#define TMR0_2  (get_timer0() & 1<<2)   // access to TMR0<2>


/***** MAIN PROGRAM *****/
void main()
{
    unsigned int8    mpx_cnt;       // multiplex counter
    unsigned int8    mins, secs;    // time counters
    
    //*** Initialisation

    // configure ports
    setup_adc_ports(NO_ANALOGS);    // disable all analog and comparator inputs
    setup_comparator(NC_NC_NC_NC);  // -> RB0, RB1, RC0, RC1 digital
        
    // configure Timer0 
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);  // timer mode, prescale = 256
                                                // -> bit 2 cycles every 2.048 ms

        
    //*** Main loop
    while (TRUE)
    {
        // count in seconds from 0:00 to 9:59
        for (mins = 0; mins < 10; mins++) 
        {
            for (secs = 0; secs < 60; secs++)
            {
                // for each time count, multiplex display for 1 second
                // (display each of 3 digits for 2.048 ms each,
                //  so repeat 1000000/2048/3 times to make 1 second) 
                for (mpx_cnt = 0; mpx_cnt < 1000000/2048/3; mpx_cnt++)
                {
                    // display minutes for 2.048 ms
                    while (!TMR0_2)         // wait for TMR0<2> to go high
                        ;
                    set7seg(mins);          // output minutes digit
                    output_high(MINUTES);   // enable minutes display
                    while (TMR0_2)          // wait for TMR0<2> to go low
                        ;
                        
                    // display tens for 2.048 ms
                    while (!TMR0_2)         // wait for TMR0<2> to go high
                        ;
                    set7seg(secs/10);       // output tens digit
                    output_high(TENS);      // enable tens display
                    while (TMR0_2)          // wait for TMR0<2> to go low
                        ;
                        
                    // display ones for 2.048 ms
                    while (!TMR0_2)         // wait for TMR0<2> to go high
                        ;
                    set7seg(secs%10);       // output ones digit
                    output_high(ONES);      // enable ones display
                    while (TMR0_2)          // wait for TMR0<2> to go low
                        ;
                }
            }
        }  
    }      
}


/***** FUNCTIONS *****/

/***** Display digit on 7-segment display *****/
void set7seg(unsigned int8 digit)
{
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
    
    // Disable displays
    output_b(0);                    // clear all digit enable lines on PORTB
    output_c(0);                    //  and PORTC
    
    // Output digit pattern
    output_b(pat7segB[digit]);      // lookup and output port B and C patterns
    output_c(pat7segC[digit]);        
}