/************************************************************************
*                                                                       *
*   Filename:      BC_L7-ADC_avg_b-CCS.c                                *
*   Date:          30/4/12                                              *
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
*   Description:    Lesson 7, example 5b                                *
*                                                                       *
*   Displays smoothed ADC output in decimal on 2x7-seg LED displays     *
*                                                                       *
*   Continuously samples analog input, averages last 16 samples,        *
*   scales result to 0 - 99 and displays as 2 x decimal digits          *
*   on multiplexed 7-segment displays.                                  *
*                                                                       *
*   Uses bank read and write functions to implement sample buffer       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       AN2             = voltage to be measured (e.g. pot or LDR)      *
*       RB0-1,RB4,RC1-4 = 7-segment display bus (common cathode)        *
*       RC5             = tens digit enable (active high)               *
*       RB5             = ones digit enable                             *
*                                                                       *
************************************************************************/

#include <16F506.h>


/***** CONFIGURATION *****/
// Config: ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,INTRC_IO,IOSC4

// Pin assignments
#define TENS_EN     PIN_C5          // tens digit enable
#define ONES_EN     PIN_B5          // ones digit enable


/***** CONSTANTS *****/
#define NSAMPLES    16              // size of sample buffer


/***** PROTOTYPES *****/
void set7seg(unsigned int8 digit);      // display digit on 7-segment display


/***** MACROS *****/
#define TMR0_2  (get_timer0() & 1<<2)   // access to TMR0<2>


/***** MAIN PROGRAM *****/
void main()
{
    int8    adc_res;                // result of ADC conversion
    int16   sum = 0;                // running total of ADC samples
    int8    adc_dec;                // scaled average (0-99)
    int8    s;                      // index into sample buffer
    
    //*** Initialisation
    
    // configure ports
    setup_comparator(NC_NC_NC_NC);  // disable comparators -> RB0-1, RC0-1 digital
    setup_vref(FALSE);              // disable CVref -> RC2 usable
                                    
    // configure ADC     
    setup_adc(ADC_CLOCK_INTERNAL);  // clock = INTOSC/4, turn ADC on
    setup_adc_ports(AN2);           // AN2 (only) analog
    set_adc_channel(2);             // select channel AN2
                                    // -> AN2 ready for sampling 
    
    // configure Timer0                            
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);  // timer mode, prescale = 256
                                                //  -> TMR0<2> cycles every 2.048 ms
    
    // clear sample buffer
    for (s = 0; s < NSAMPLES; s++)
        write_bank(1,s,0);   
    
        
    //*** Main loop
    while (TRUE)
    {
        for (s = 0; s < NSAMPLES; s++)
        {
            // sample input
            adc_res = read_adc();
        
            // update running total
            sum += (int16)adc_res - read_bank(1,s); // add new value and subtract old
            write_bank(1,s,adc_res);                // update buffer with new value
            
            // calculate average and scale to 0-99
            adc_dec = sum / NSAMPLES * 100/256;

            // display tens digit for 2.048 ms
            while (!TMR0_2)             // wait for TMR0<2> to go high
                ;
            set7seg(adc_dec/10);        // output tens digit of result
            output_high(TENS_EN);       // enable tens digit display 
            while (TMR0_2)              // wait for TMR0<2> to go low
                ;
                        
            // display ones digit for 2.048 ms
            while (!TMR0_2)             // wait for TMR0<2> to go high
                ;
            set7seg(adc_dec%10);        // output ones digit of result
            output_high(ONES_EN);       // enable ones digit display
            while (TMR0_2)              // wait for TMR0<2> to go low
                ;
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
    
    // disable displays
    output_b(0);                    // clear all digit enable lines on PORTB
    output_c(0);                    //  and PORTC
    
    // output digit pattern
    output_b(pat7segB[digit]);      // lookup and output port B and C patterns
    output_c(pat7segC[digit]);        
}
