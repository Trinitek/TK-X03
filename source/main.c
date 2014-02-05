/* FILES TO INCLUDE */

/* Abstract pin definitions and system configuration settings are found in
 * "configuration_bits.c" */

#include "configuration_bits.c"
#include <xc.h>             // XC8 General Include File
#include <stdint.h>         // For uint8_t definition
#include <stdbool.h>        // For true/false definition

/* PROTOTYPES */
void initPorts(void);
void wait_ms(uint16_t);
void dispSeg(uint8_t, uint8_t);
void cycleSegDisplays(uint16_t);
void enableSerialRX(uint16_t);

void main(void)
{
    initPorts();
    enableSerialRX(1200);
    dispSeg(1, '-'); dispSeg(2, '-');

    while(1) {
        while(PIR3bits.RC2IF == 0) continue;
                            // hang until RX byte is transferred to buffer
        uint8_t receivedByte = rxByte ^ 0xF0;
                            // perform XOR to clear top nibble
        dispSeg(1, receivedByte);

        switch (receivedByte) {
            case 10:
                break;
            case 11:
                break;
        }
    }

    /*
    dispSeg(1, '-'); dispSeg(1, '-');
    
    while (1)
    {
        if (serialRX == 1) dispSeg(1, '1');
        if (serialRX == 0) dispSeg(1, '0');
        else dispSeg(1, 'E');
    }
    */

    //while(1) continue;

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
    TRISDbits.RD6 = 0;  // RS-232 TX outgoing (out)
    TRISDbits.RD7 = 1;  // RS-232 RX incoming (in)

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
 * Display a hexadecimal number on either of the two segment displays.
 *
 * @param segment - number identifier of the segment display
 * @param letter - hexadecimal character to display, 0 to turn off
 */
void dispSeg(uint8_t segment, uint8_t letter)
{
    switch (letter) {
        case 'x' | 'X':     // turn off display
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = seg1g = 0;
            }
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = seg2g = 0;
            break;
        case '-':           // display dash
            if (segment == 1) {
                seg1g = 1;
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = 0;
            }
            seg2g = 1;
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = 0;
            break;
        case '0' | 0x0:     // display 0
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = 1;
                seg1g = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = 1;
            seg2g = 0;
            break;
        case '1' | 0x1:     // display 1
            if (segment == 1) {
                seg1b = seg1c = 1;
                seg1a = seg1d = seg1e = seg1f = seg1g = 0;
                return;
            }
            seg2b = seg2c = 1;
            seg2a = seg2d = seg2e = seg2f = seg2g = 0;
            break;
        case '2' | 0x2:     // display 2
            if (segment == 1) {
                seg1a = seg1b = seg1g = seg1e = seg1d = 1;
                seg1c = seg1f = 0;
                return;
            }
            seg2a = seg2b = seg2g = seg2e = seg2d = 1;
            seg2c = seg2f = 0;
            break;
        case '3' | 0x3:     // display 3
            if (segment == 1) {
                seg1a = seg1b = seg1g = seg1c = seg1d = 1;
                seg1e = seg1f = 0;
                return;
            }
            seg2a = seg2b = seg2g = seg2c = seg2d = 1;
            seg2e = seg2f = 0;
            break;
        case '4' | 0x4:     // display 4
            if (segment == 1) {
                seg1f = seg1g = seg1b = seg1c = 1;
                seg1a = seg1d = seg1e = 0;
                return;
            }
            seg2f = seg2g = seg2b = seg2c = 1;
            seg2a = seg2d = seg2e = 0;
            break;
        case '5' | 0x5:     // display 5
            if (segment == 1) {
                seg1a = seg1f = seg1g = seg1c = seg1d = 1;
                seg1b = seg1e = 0;
                return;
            }
            seg2a = seg2f = seg2g = seg2c = seg2d = 1;
            seg2b = seg2e = 0;
            break;
        case '6' | 0x6:     // display 6
            if (segment == 1) {
                seg1a = seg1f = seg1g = seg1c = seg1d = seg1e = 1;
                seg1b = 0;
                return;
            }
            seg2a = seg2f = seg2g = seg2c = seg2d = seg2e = 1;
            seg2b = 0;
            break;
        case '7' | 0x7:     // display 7
            if (segment == 1) {
                seg1a = seg1b = seg1c = 1;
                seg1d = seg1e = seg1f = seg1g = 0;
                return;
            }
            seg2a = seg2b = seg2c = 1;
            seg2d = seg2e = seg2f = seg2g = 0;
            break;
        case '8' | 0x8:     // display 8
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = seg1g = 1;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = seg2g = 1;
            break;
        case '9' | 0x9:     // display 9
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1f = seg1g = 1;
                seg1e = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2f = seg2g = 1;
            seg2e = 0;
            break;
        case 'a' | 'A' | 10:  // display A
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1e = seg1f = seg1g = 1;
                seg1d = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2e = seg2f = seg2g = 1;
            seg2d = 0;
            break;
        case 'b' | 'B':// | 11:  // display B
            if (segment == 1) {
                seg1c = seg1d = seg1e = seg1f = seg1g = 1;
                seg1a = seg1b = 0;
                return;
            }
            seg2c = seg2d = seg2e = seg2f = seg2g = 1;
            seg2a = seg2b = 0;
            break;
        case 'c' | 'C':   // display C
            if (segment == 1) {
                seg1a = seg1d = seg1e = seg1f = 1;
                seg1b = seg1c = seg1g = 0;
                return;
            }
            seg2a = seg2d = seg2e = seg2f = 1;
            seg2b = seg2c = seg2g = 0;
            break;
        case 'd' | 'D':   // display D
            if (segment == 1) {
                seg1b = seg1c = seg1d = seg1e = seg1g = 1;
                seg1a = seg1f = 0;
                return;
            }
            seg2b = seg2c = seg2d = seg2e = seg2g = 1;
            seg2a = seg2f = 0;
            break;
        case 'e' | 'E':   // display E
            if (segment == 1) {
                seg1a = seg1d = seg1e = seg1f = seg1g = 1;
                seg1b = seg1c = 0;
                return;
            }
            seg2a = seg2d = seg2e = seg2f = seg2g = 1;
            seg2b = seg2c = 0;
            break;
        case 'f' | 'F':   // display F
            if (segment == 1) {
                seg1a = seg1e = seg1f = seg1g = 1;
                seg1b = seg1c = seg1d = 0;
                return;
            }
            seg2a = seg2e = seg2f = seg2g = 1;
            seg2b = seg2c = seg2d = 0;
            break;
    }
}

/**
 * Cycle through the characters that both segment displays can show.
 *
 * @param time - Wait period between characters
 */
void cycleSegDisplays(uint16_t time)
{
    dispSeg(1, 'x'); dispSeg(2, 'x');
    wait_ms(time);
    dispSeg(1, '-'); dispSeg(2, '-');
    wait_ms(time);
    dispSeg(1, '0'); dispSeg(2, '0');
    wait_ms(time);
    dispSeg(1, '1'); dispSeg(2, '1');
    wait_ms(time);
    dispSeg(1, '2'); dispSeg(2, '2');
    wait_ms(time);
    dispSeg(1, '3'); dispSeg(2, '3');
    wait_ms(time);
    dispSeg(1, '4'); dispSeg(2, '4');
    wait_ms(time);
    dispSeg(1, '5'); dispSeg(2, '5');
    wait_ms(time);
    dispSeg(1, '6'); dispSeg(2, '6');
    wait_ms(time);
    dispSeg(1, '7'); dispSeg(2, '7');
    wait_ms(time);
    dispSeg(1, '8'); dispSeg(2, '8');
    wait_ms(time);
    dispSeg(1, '9'); dispSeg(2, '9');
    wait_ms(time);
    dispSeg(1, 'a'); dispSeg(2, 'a');
    wait_ms(time);
    dispSeg(1, 'b'); dispSeg(2, 'b');
    wait_ms(time);
    dispSeg(1, 'c'); dispSeg(2, 'c');
    wait_ms(time);
    dispSeg(1, 'd'); dispSeg(2, 'd');
    wait_ms(time);
    dispSeg(1, 'e'); dispSeg(2, 'e');
    wait_ms(time);
    dispSeg(1, 'f'); dispSeg(2, 'f');
    wait_ms(time);
}

/**
 * Initialize the UART to receive data from the RS-232 port.
 *
 * ...
 * 
 * Analog functionality already disabled, and direction settings are set.
 *
 * The receiver looks for a single START bit, and a single STOP bit. No
 * error is triggered on a failed START bit, but a framing error is set on
 * failure to detect a valid STOP bit.
 *
 * If the FIFO buffer is overrun, the condition must be cleared by clearing the
 * CREN bit and resetting it before another byte can be captured.
 */
void enableSerialRX(uint16_t baudRate)
{
    serialEn = 1;           // enable RS-232 interface

    /* Set the baud rate; selectable values listed: */
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
    }

    TXSTA2bits.BRGH = 0;    // set the baud rate multiplier bits
    BAUDCON2bits.BRG16 = 0;

    RCSTA2bits.CREN = 1;    // enable onboard receiver
    TXSTA2bits.SYNC = 0;    // asynchronous operation
    RCSTA2bits.SPEN = 1;    // enable onboard UART
    BAUDCON2bits.DTRXP = 1; // inverse logic polarity (1 = logic 1)
}