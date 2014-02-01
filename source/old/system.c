/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "system.h"

void ConfigureOscillator(void)
{
    OSCCONbits.IRCF = 0b111;    //0b101;    // 4 MHz clock
    OSCCONbits.SCS = 0b10;      // internal clock
    OSCTUNEbits.PLLEN = 0b0;    //0b1;    // multiply freq *4 to get 16 MHz
}

/**
 * Wait for a given number of milliseconds using busy waiting scheme
 * @param time - time in ms to wait.
 */
void wait_ms(uint16_t time) {
    static long timel = 0;
    timel = time * 126l;        // calibrated for 1000 ms ~=1 second
    for (; timel; timel--);
}