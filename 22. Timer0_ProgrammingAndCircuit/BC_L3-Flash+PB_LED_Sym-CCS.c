/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Flash+PB_LED_Sym-CCS.c                         *
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
*   Description:    Lesson 3, example 2a                                *
*                                                                       *
*   Demonstates use of Timer0 to maintain timing of background actions  *
*   while performing other actions in response to changing inputs       *
*                                                                       *
*   One LED simply flashes at 1 Hz (50% duty cycle).                    *
*   The other LED is only lit when the pushbutton is pressed            *
*                                                                       *
*   Uses symbols to represent port bit positions                        *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = "button pressed" indicator LED                            *
*       GP2 = flashing LED                                              *
*       GP3 = pushbutton switch (active low)                            *
*                                                                       *
************************************************************************/

#include <12F509.h>

#define GP0 PIN_B0              // define GP pins
#define GP1 PIN_B1
#define GP2 PIN_B2
#define GP3 PIN_B3
#define GP4 PIN_B4
#define GP5 PIN_B5


/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
#fuses NOMCLR,NOPROTECT,NOWDT,INTRC

// Pin assignments
#define nFLASH  2               // flashing LED on GP2
#define nPRESS  1               // "button pressed" indicator LED on GP1

#define BUTTON  GP3             // pushbutton


/***** GLOBAL VARIABLES *****/
unsigned int8   sGPIO;              // shadow copy of GPIO


/***** MAIN PROGRAM *****/
void main()
{
    unsigned int8   dc;             // delay counter

    // Initialisation
    // configure ports
    output_b(0);                    // start with all LEDs off
    sGPIO = 0;                      //   update shadow
    // configure Timer0 
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32);   // timer mode, prescale = 32
                                                // -> increment every 32 us

    // Main loop  
    while (TRUE)
    {
        // delay 500 ms while responding to button press
        for (dc = 0; dc < 125; dc++)     // repeat for 500ms (125 x 4ms = 500ms)
        {  
            set_timer0(0);               //   clear timer0
            while (get_timer0() < 125)   //   repeat for 4ms (125 x 32us)
            {                            //     check and respond to button press
                sGPIO &= ~(1<<nPRESS);   //       assume button up -> indicator LED off
                if (input(BUTTON) == 0)  //       if button pressed (low)
                    sGPIO |= 1<<nPRESS;  //         turn on indicator LED
                output_b(sGPIO);         //     update port (copy shadow to GPIO)
            }
        }
        // toggle flashing LED       
        sGPIO ^= 1<<nFLASH;              // toggle flashing LED, using shadow register
               
    }   // repeat forever
}
