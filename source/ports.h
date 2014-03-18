/* Virtual port definitions for the TK3081 processor emulator */

/* Serial port I/O and UART control */

#define UART_BAUD           0x00            // o: set UART baud rate
#define UART_SEND           0x01            // o: send byte to UART
#define UART_RCV            0x02            // i: receive byte from UART


/* Segment display control */

#define SEG_A               0x03            // o: send byte to display A
#define SEG_B               0x04            // o: send byte to display B