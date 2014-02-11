/* RS-232 serial interface control */

#include <stdint.h>                 // For uint8_t definition
#include "configuration_bits.c"     // For pin definitions

/**
 * Initialize the UART to receive data from the RS-232 port.
 *
 * ...
 *
 * Analog functionality already disabled.
 *
 * The receiver looks for a single START bit, and a single STOP bit. No
 * error is triggered on a failed START bit, but a framing error is set on
 * failure to detect a valid STOP bit.
 *
 * If the FIFO buffer is overrun, the condition must be cleared by clearing the
 * CREN bit and resetting it before another byte can be captured.
 */
void enableSerial(uint16_t baudRate)
{
    serialEn = 1;           // enable RS-232 interface

    /* Set the baud rate; selectable values listed (1200 default): */
    switch (baudRate) {
        case 1200:          // 1200 baud, 0.16% error margin
            SPBRG2 = 207;
            break;
        case 2400:          // 2400 baud, 0.16% error margin
            SPBRG2 = 103;
            break;
        case 9600:          // 9600 baud, 0.16% error margin
            SPBRG2 = 25;
            break;
        case 10417:         // 10417 baud, 0.00% error margin
            SPBRG2 = 23;
            break;
        case 19200:         // 19200 baud, 0.16% error margin
            SPBRG2 = 12;
            break;
        default:
            SPBRG2 = 207;   // 1200 baud default if no valid rate selected
    }

    SPBRGH2 = 0;            // clear the high byte of the baud rate select
    TXSTA2bits.BRGH = 0;    // set the baud rate multiplier bits
    BAUDCON2bits.BRG16 = 0;

    TXSTA2bits.TX9 = 0;     // 8-bit TX send
    RCSTA2bits.RX9 = 0;     // 8-bit RX receive

    TXSTA2bits.SYNC = 0;    // asynchronous operation
    BAUDCON2bits.DTRXP = 0; // do not invert incoming data

    TXSTA2bits.TXEN = 1;    // enable onboard transmitter
    RCSTA2bits.CREN = 1;    // enable onboard receiver

    RCSTA2bits.SPEN = 1;    // enable onboard UART and serial function on pins
}