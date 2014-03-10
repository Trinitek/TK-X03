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
#include "registers.h"              // For TK3081 register and

//struct immData_t immData;           // immediate data to be passed to instruction decoder

void main(void)
{
    initOscillator();                   // initialize oscillator configuration
    initPorts();                        // initialize pin direction and function
    dispSeg(1, '-'); dispSeg(2, '-');

    initializeRegisters();              // initialize virtual processor registers

    while (1)
    {
        decode_immData(immData);               // determine where the immediate data is and load that data into temporary vars

        if (regPC < memReadOnly || regPC > memScratchPad_e)
        {
            regPC = memReadOnly;               // soft reset if PC overflows into stack or out of virtual memory boundaries
            continue;
        }

        if (regPC > memStack_e && regPC < memScratchPad) processOpcode(virtualMemory[regPC], immData);
        if (regPC > memReadOnly_e) processOpcode(virtualMemory[regPC], immData);
    }
}