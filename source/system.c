/* Hardware and oscillator configuration code */

#include <xc.h>                     // XC8 General Include File
#include <stdint.h>                 // For uint8_t definition

/**
 * Initialize the internal oscillator
 */
void initOscillator(void)
{
    OSCCONbits.IDLEN = 0;       // sleep on SLEEP instruction
    OSCCONbits.IRCF = 0b111;    // 16 MHz internal RC oscillator
    OSCCONbits.SCS = 0b10;      // set internal block as system oscillator
    OSCCON2bits.MFIOSEL = 0;    // low frequency block disabled
    OSCCON2bits.SOSCGO = 0;     // secondary oscillator disabled
    OSCCON2bits.PRISD = 0;      // oscillator power circuit off

    // hang until frequency is stable
    while (OSCCONbits.HFIOFS == 0) continue;
}

/**
 * Initialize all of the physically connected and used pins on the controller,
 * and specify their I/O direction and their functionality.
 */
void initPorts(void)
{
    TRISA = 0;          // set all A, C, and E pins to outputs
    TRISC = 0;
    TRISE = 0;          // (RE3 is static input

    ANSELD = 0;         // disable analog functionality on D pins
    TRISDbits.RD3 = 0;  // RS-232 interface enable (out)
    TRISDbits.RD4 = 1;  // RS-232 clear to send (in)
    TRISDbits.RD5 = 0;  // RS-232 request to send (out)
    TRISDbits.RD7 = 1;  // RS-232 RX receive (in)
    TRISDbits.RD6 = 0;  // RS-232 TX send (out)

    LATA = 0;           // clear flip-flops
    LATC = 0;
    LATD = 0;
    LATE = 0;
}

/**
 * Wait for a given number of milliseconds using busy waiting scheme.
 *
 * @param time - time in ms to wait
 */
void wait_ms(uint16_t time)
{
    static long timel = 0;
    timel = time * 10; //126l;        // calibrated for 1000 ms ~=1 second
    for (; timel; timel--);
}

/**
 * Converts the lower nibble of the passed number into a UTF-8 character.
 *
 * @param number - only lower nibble read
 * @return character - UTF-8 representation in lower nibble
 */
uint8_t hexToChar(uint8_t number)
{
    number = number << 4;           // shift top nibble left and back to clear
    number = number >> 4;           // number == (binary)0000_xxxx
    switch (number)
    {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 0xA:
            return 'A';
        case 0xB:
            return 'B';
        case 0xC:
            return 'C';
        case 0xD:
            return 'D';
        case 0xE:
            return 'E';
        case 0xF:
            return 'F';
    }
    return 0;
}