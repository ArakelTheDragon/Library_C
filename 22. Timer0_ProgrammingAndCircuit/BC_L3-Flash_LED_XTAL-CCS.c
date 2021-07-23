/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Flash_LED_XTAL-CCS.c                           *
*   Date:                                                               *
*   File Version:  1.2                                                  *
*                                                                       *
*   Author:        Yordan Itsov                                         *
*   Company:                                                            *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     12F508/509                                           *
*   Compiler:      CCS PCB v5.015                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 3, example 4                                 *
*                                                                       *
*   Demonstrates use of Timer0 in counter mode                          *
*                                                                       *
*   LED flashes at 1 Hz (50% duty cycle),                               *
*   with timing derived from 32.768 kHz input on T0CKI                  *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1   = flashing LED                                            *
*       T0CKI = 32.768 kHz signal                                       *
*                                                                       *
************************************************************************/

#include <12F509.h>

#bit TMR0_7 = 0x01.7                // bit 7 of TMR0


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, int RC clock 
#fuses MCLR,NOPROTECT,NOWDT,INTRC

// Pin assignments
#define sFLASH  sGPIO.GP1           // flashing LED (shadow)


/***** GLOBAL VARIABLES *****/
union {                             // shadow copy of GPIO
    unsigned int8   port;
    struct {
        unsigned    GP0     : 1;
        unsigned    GP1     : 1;
        unsigned    GP2     : 1;
        unsigned    GP3     : 1;
        unsigned    GP4     : 1;
        unsigned    GP5     : 1;
    };
} sGPIO;


/***** MAIN PROGRAM *****/
void main()
{
    // Initialisation
    
    // configure Timer0 
    setup_timer_0(RTCC_EXT_L_TO_H|RTCC_DIV_128);  // counter mode, prescale = 128
                                                  // -> increment at 256 Hz
                                                  //    with 32.768 kHz input

    // Main loop
    while (TRUE)
    {
        // TMR0<7> cycles at 1 Hz, so continually copy to LED
        sFLASH = TMR0_7;
        
        output_b(sGPIO.port);       // copy shadow to GPIO
        
    }   // repeat forever
}
