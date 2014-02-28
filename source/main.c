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
#include "emulator.h"               // For TK3081 processor emulator

struct immData_t immData;           // immediate data to be passed to instruction decoder

void main(void)
{
    initOscillator();                   // initialize oscillator configuration
    initPorts();                        // initialize pin direction and function
    //enableSerial(1200);                 // initialize serial TX/RX at 1200 baud
    dispSeg(1, '-'); dispSeg(2, '-');

    initializeRegisters();              // initialize virtual processor registers

    while (1)
    {
        decode_immData(immData);               // determine where the immediate data is and load that data into temporary vars

        if (regPC < 256)
        {
            regPC = 256;                // soft reset if PC overflows into stack
            continue;
        }

        if (regPC > 255 && regPC < 512) processOpcode(memoryMap.readOnly[regPC], immData);
        if (regPC > 511) processOpcode(memoryMap.scratchPad[regPC], immData);
    }
}