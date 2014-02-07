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
uint8_t hexToChar(uint8_t);
void dispSeg(uint8_t, uint8_t);
void cycleSegDisplays(uint16_t);
void enableSerial(uint16_t);
void isr(void);

void main(void)
{
    initPorts();                        // initialize pin direction and function
    enableSerial(1200);                 // initialize serial TX/RX at 1200 baud
    dispSeg(1, '-'); dispSeg(2, '-');

    while(1)
    {
           if(PIR3bits.RC2IF == 1)      // is there something in the RX buffer?
      {
            if(PIR3bits.TX2IF == 1)     // is the TX buffer clear?
            {
                txByte = 'A';
            }
        }
    }
    /*{
        // hang until byte is received
        if (PIR3bits.RC2IF == 1)
        {
            // grab byte and convert to character to display
            //uint8_t receivedByte = hexToChar(rxByte);
            //dispSeg(1, receivedByte);
            dispSeg(1, '1');

            if (PIR3bits.TX2IF == 1)
            {
                dispSeg(2, '1');
                txByte = rxByte;
            }
            else dispSeg(2, '0');
        }
        else dispSeg(1, '0');

        // check for FIFO error before reading
        /*if (RCSTA2bits.OERR == 1)
        {
            // if FIFO overflow error set, reset receiver
            dispSeg(2, '0');
            RCSTA2bits.CREN = 0; RCSTA2bits.CREN = 1;
        }
    }*/
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

/**
 * Display a hexadecimal number on either of the two segment displays.
 *
 * @param segment - number identifier of the segment display
 * @param letter - hexadecimal character to display, 0 or 'x' to turn off
 */
void dispSeg(uint8_t segment, uint8_t letter)
{
    switch (letter) {
        case 'x' | 'X' | 0:     // turn off display
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = seg1g = 0;
            }
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = seg2g = 0;
            break;
        case '-':               // display dash
            if (segment == 1) {
                seg1g = 1;
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = 0;
            }
            seg2g = 1;
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = 0;
            break;
        case '0':               // display 0
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = 1;
                seg1g = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = 1;
            seg2g = 0;
            break;
        case '1':               // display 1
            if (segment == 1) {
                seg1b = seg1c = 1;
                seg1a = seg1d = seg1e = seg1f = seg1g = 0;
                return;
            }
            seg2b = seg2c = 1;
            seg2a = seg2d = seg2e = seg2f = seg2g = 0;
            break;
        case '2':               // display 2
            if (segment == 1) {
                seg1a = seg1b = seg1g = seg1e = seg1d = 1;
                seg1c = seg1f = 0;
                return;
            }
            seg2a = seg2b = seg2g = seg2e = seg2d = 1;
            seg2c = seg2f = 0;
            break;
        case '3':               // display 3
            if (segment == 1) {
                seg1a = seg1b = seg1g = seg1c = seg1d = 1;
                seg1e = seg1f = 0;
                return;
            }
            seg2a = seg2b = seg2g = seg2c = seg2d = 1;
            seg2e = seg2f = 0;
            break;
        case '4':               // display 4
            if (segment == 1) {
                seg1f = seg1g = seg1b = seg1c = 1;
                seg1a = seg1d = seg1e = 0;
                return;
            }
            seg2f = seg2g = seg2b = seg2c = 1;
            seg2a = seg2d = seg2e = 0;
            break;
        case '5':               // display 5
            if (segment == 1) {
                seg1a = seg1f = seg1g = seg1c = seg1d = 1;
                seg1b = seg1e = 0;
                return;
            }
            seg2a = seg2f = seg2g = seg2c = seg2d = 1;
            seg2b = seg2e = 0;
            break;
        case '6':               // display 6
            if (segment == 1) {
                seg1a = seg1f = seg1g = seg1c = seg1d = seg1e = 1;
                seg1b = 0;
                return;
            }
            seg2a = seg2f = seg2g = seg2c = seg2d = seg2e = 1;
            seg2b = 0;
            break;
        case '7':               // display 7
            if (segment == 1) {
                seg1a = seg1b = seg1c = 1;
                seg1d = seg1e = seg1f = seg1g = 0;
                return;
            }
            seg2a = seg2b = seg2c = 1;
            seg2d = seg2e = seg2f = seg2g = 0;
            break;
        case '8':               // display 8
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = seg1g = 1;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = seg2g = 1;
            break;
        case '9':               // display 9
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1f = seg1g = 1;
                seg1e = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2f = seg2g = 1;
            seg2e = 0;
            break;
        case 'a' | 'A':         // display A
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1e = seg1f = seg1g = 1;
                seg1d = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2e = seg2f = seg2g = 1;
            seg2d = 0;
            break;
        case 'b' | 'B':         // display B
            if (segment == 1) {
                seg1c = seg1d = seg1e = seg1f = seg1g = 1;
                seg1a = seg1b = 0;
                return;
            }
            seg2c = seg2d = seg2e = seg2f = seg2g = 1;
            seg2a = seg2b = 0;
            break;
        case 'c' | 'C':         // display C
            if (segment == 1) {
                seg1a = seg1d = seg1e = seg1f = 1;
                seg1b = seg1c = seg1g = 0;
                return;
            }
            seg2a = seg2d = seg2e = seg2f = 1;
            seg2b = seg2c = seg2g = 0;
            break;
        case 'd' | 'D':         // display D
            if (segment == 1) {
                seg1b = seg1c = seg1d = seg1e = seg1g = 1;
                seg1a = seg1f = 0;
                return;
            }
            seg2b = seg2c = seg2d = seg2e = seg2g = 1;
            seg2a = seg2f = 0;
            break;
        case 'e' | 'E':         // display E
            if (segment == 1) {
                seg1a = seg1d = seg1e = seg1f = seg1g = 1;
                seg1b = seg1c = 0;
                return;
            }
            seg2a = seg2d = seg2e = seg2f = seg2g = 1;
            seg2b = seg2c = 0;
            break;
        case 'f' | 'F':         // display F
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
    dispSeg(1, 0); dispSeg(2, 0);
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
    BAUDCON2bits.DTRXP = 1; // inverse logic polarity (active low = logic 1)
  
    TXSTA2bits.TXEN = 1;    // enable onboard transmitter
    RCSTA2bits.CREN = 1;    // enable onboard receiver

    RCSTA2bits.SPEN = 1;    // enable onboard UART and serial function on pins
}

void isr(void)
{
    
}