/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Flash+PB_LED_Union-CCS.c                       *
*   Date:                                                               *
*   File Version:  1.1                                                  *
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
*   Description:    Lesson 3, example 2b                                *
*                                                                       *
*   Demonstrates use of Timer0 to maintain timing of background actions *
*   while performing other actions in response to changing inputs       *
*                                                                       *
*   One LED simply flashes at 1 Hz (50% duty cycle).                    *
*   The other LED is only lit when the pushbutton is pressed            *
*                                                                       *
*   Uses union / bitfield structure to represent shadow register        *
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


/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
#fuses NOMCLR,NOPROTECT,NOWDT,INTRC

// Pin assignments
#define sFLASH  sGPIO.GP2       // flashing LED (shadow)
#define sPRESS  sGPIO.GP1       // "button pressed" indicator LED (shadow)

#define BUTTON  PIN_B3          // pushbutton on GP3


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
    unsigned int8   dc;             // delay counter

    // Initialisation
    // configure ports
    output_b(0);                    // start with all LEDs off
    sGPIO.port = 0;                 //   update shadow
    // configure Timer0 
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32);   // timer mode, prescale = 32
                                                // -> increment every 32 us

    // Main loop  
    while (TRUE)
    {
        // delay 500 ms while responding to button press
        for (dc = 0; dc < 125; dc++)    // repeat for 500ms (125 x 4ms = 500ms)
        {  
            set_timer0(0);              //   clear timer0
            while (get_timer0() < 125)  //   repeat for 4ms (125 x 32us)
            {                      
                sPRESS = !input(BUTTON);    // turn on indicator only if button pressed 
                output_b(sGPIO.port);       // update port (copy shadow to GPIO)
            }
        }
        // toggle flashing LED       
        sFLASH = !sFLASH;               // toggle flashing LED (shadow)
               
    }   // repeat forever
}
