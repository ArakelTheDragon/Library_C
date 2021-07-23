/************************************************************************
*                                                                       *
*   Filename:      BC_L7-ADC_Vdd-measure_f-CCS.c                        *
*   Date:          26/4/12                                              *
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
*   Description:    Lesson 7, example 3a                                *
*                                                                       *
*   Demonstrates use of 0.6 V ref with ADC to test supply voltage       *
*                                                                       *
*   Continuously samples 0.6 V internal reference,                      *
*   displaying result as 2 x hex digits on multiplexed 7-seg displays   *
*   Turns on warning LED if measurement > threshold                     *
*   (using floating-point comparison)                                   *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       RB0-1,RB4,RC1-4 = 7-segment display bus (common cathode)        *
*       RC5             = "tens" digit enable (active high)             *
*       RB5             = ones digit enable                             *
*       RC0             = warning LED                                   *
*                                                                       *
************************************************************************/

#include <16F506.h>


/***** CONFIGURATION *****/
// Config: ext reset, no code protect, no watchdog, 4 MHz int clock
#fuses MCLR,NOPROTECT,NOWDT,INTRC_IO,IOSC4

// Pin assignments
#define TENS_EN     PIN_C5          // "tens" (high nybble) digit enable
#define ONES_EN     PIN_B5          // ones digit enable
#define WARN        PIN_C0          // warning LED


/***** CONSTANTS *****/
#define MINVDD  3.5                 // minimum Vdd (Volts)


/***** PROTOTYPES *****/
void set7seg(unsigned int8 digit);      // display digit on 7-segment display


/***** MACROS *****/
#define TMR0_2  (get_timer0() & 1<<2)   // access to TMR0<2>


/***** MAIN PROGRAM *****/
void main()
{
    unsigned int8   adc_res;        // result of ADC conversion
    
    //*** Initialisation
    
    // configure ports
    setup_comparator(NC_NC_NC_NC);  // disable comparators -> RB0-1, RC0-1 digital
    setup_vref(FALSE);              // disable CVref -> RC2 usable

    // configure ADC:
    setup_adc(ADC_CLOCK_INTERNAL);  // clock = INTOSC/4, turn ADC on      
    setup_adc_ports(NO_ANALOGS);    // no analog inputs -> RB0-2 digital 
    set_adc_channel(3);             // select 0.6 V reference 
                                    // -> 0.6 V reference ready for sampling
    
    // configure Timer0                             
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);  // timer mode, prescale = 256
                                                //  -> TMR0<2> cycles every 2.048 ms
                                       
    //*** Main loop
    while (TRUE)
    {
        // sample 0.6 V reference
        adc_res = read_adc();
        
        // test for low Vdd
        if (adc_res > 0.6/MINVDD*255)       // if measured 0.6 V > threshold
            output_high(WARN);              //   light warning LED
            
        // display high nybble for 2.048 ms
        while (!TMR0_2)           // wait for TMR0<2> to go high
            ;
        set7seg(adc_res >> 4);    // output high nybble of result
        output_high(TENS_EN);     // enable "tens" digit 
        while (TMR0_2)            // wait for TMR0<2> to go low
            ;
                        
        // display low nybble for 2.048 ms
        while (!TMR0_2)           // wait for TMR0<2> to go high
            ;
        set7seg(adc_res & 0x0F);  // output low nybble of result 
        output_high(ONES_EN);     // enable ones digit
        while (TMR0_2)            // wait for TMR0<2> to go low
            ;
    }      
}


/***** FUNCTIONS *****/

/***** Display digit on 7-segment display *****/
void set7seg(unsigned int8 digit)
{
    // pattern table for 7 segment display on port B
    const int8 pat7segB[16] = {
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
        0b000011,   // 9 
        0b010011,   // A
        0b010011,   // b
        0b010010,   // C
        0b010001,   // d
        0b010011,   // E
        0b010011    // F    
    }; 

    // pattern table for 7 segment display on port C
    const int8 pat7segC[16] = {
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
        0b011110,   // 9
        0b010110,   // A
        0b011000,   // b
        0b001010,   // C
        0b011100,   // d
        0b001010,   // E
        0b000010    // F
    };
    
    // disable displays
    output_b(0);                    // clear all digit enable lines on PORTB
    output_c(0);                    //  and PORTC
    
    // output digit pattern
    output_b(pat7segB[digit]);      // lookup and output port B and C patterns
    output_c(pat7segC[digit]);        
}
