/* Virtual port definitions for the TK3081 processor emulator */

//#include <xc.h>                     // XC8 General Include File
//#include <stdint.h>                 // For uint8_t definition
//#include "registers.c"              // For register, opcode, and memory definitions


/* Serial port I/O and UART control */

#define UART_BAUD           0b00000000      // o: set UART baud rate
#define UART_SEND           0b00000001      // o: send byte to UART
#define UART_RCV            0b00000010      // i: receive byte from UART


/* Segment display control */

#define SEG_A               0b00000011      // o: send byte to display A
#define SEG_B               0b00000100      // o: send byte to display B