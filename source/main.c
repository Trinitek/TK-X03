/* FILES TO INCLUDE */

/* Abstract pin definitions and system configuration settings are found in
 * "configuration_bits.c" */

#include <xc.h>                     // XC8 General Include File
#include <stdint.h>                 // For uint8_t definition
#include <stdbool.h>                // For true/false definition
#include "configuration_bits.c"     // For pin definitions and fuse settings
#include "system.h"                 // For system settings and basic functions
#include "segment.h"                // For segment display handling
#include "serial.h"                 // For RS-232 serial controller

/* PROTOTYPES */
uint8_t hexToChar(uint8_t);

void main(void)
{
    initOscillator();                   // initialize oscillator configuration
    initPorts();                        // initialize pin direction and function
    enableSerial(1200);                 // initialize serial TX/RX at 1200 baud
    dispSeg(1, '-'); dispSeg(2, '-');

    while(1)
    {
           if(PIR3bits.RC2IF == 1)      // is there something in the RX buffer?
      {
            if(PIR3bits.TX2IF == 1)     // is the TX buffer clear?
            {
                if(RCSTA2bits.FERR2 == 1)   // check for stop error
                {
                    dispSeg(1, 'F');
                }

                txByte = rxByte;        // echo received byte
            }
        }
    }
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