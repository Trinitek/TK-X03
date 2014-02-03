/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>             // XC8 General Include File
#include <stdint.h>         // For uint8_t definition
#include <stdbool.h>        // For true/false definition

/* PROTOTYPES */
void wait_ms(uint16_t);
void dispSeg(char, char);
void cycleSegDisplays(uint16_t);

/* Pin definitions for the individual LED segments
 * ...............
 * .....=====.A...
 * ..F.|.....|.B..
 * ...G.=====.....
 * ..E.|.....|.C..
 * .....=====.D...
 * ...............
 */
#define seg1a       LATAbits.LA2
#define seg1b       LATAbits.LA3
#define seg1c       LATAbits.LA4
#define seg1d       LATAbits.LA5
#define seg1e       LATEbits.LE0
#define seg1f       LATAbits.LA0
#define seg1g       LATAbits.LA1

#define seg2a       LATAbits.LA7
#define seg2b       LATAbits.LA6
#define seg2c       LATCbits.LC2
#define seg2d       LATCbits.LC0
#define seg2e       LATCbits.LC1
#define seg2f       LATEbits.LE1
#define seg2g       LATEbits.LE2

/* Pin definitions for the RS-232 interface controller */
#define serialEn    LATDbits.LD3    // enable controller
#define serialTX    LATDbits.LD5    //LATDbits.LD6    // TX outgoing
#define serialRX    PORTDbits.RD6   //PORTDbits.RD7   // RX incoming
#define serialREQ   LATDbits.LD4    //LATDbits.LD5    // request to send
#define serialOK    PORTCbits.RC7   //PORTDbits.RD4   // clear to send

void main(void)
{

    TRISA = 0;          // set all A, C, and E pins to outputs
    TRISC = 0;
    TRISE = 0;          // (RE3 is static input

    /* TODO :: reconfigure physical connections going to the RS-232 interface!*/
    ANSELD = 0;         // disable analog functionality on D pins
    TRISDbits.RD3 = 0;  // RS-232 interface enable (out)
    //TRISDbits.RD5 = 0;  // RS-232 request to send (out)
    TRISDbits.RD4 = 0;  // IMPROPERLY CONNECTED
    //TRISDbits.RD4 = 1;  // RS-232 clear to send (in)
    TRISCbits.RC7 = 1;  // IMPROPERLY CONNECTED
    //TRISDbits.RD6 = 0;  // RS-232 TX outgoing (out)
    TRISDbits.RD5 = 0;  // IMPROPERLY CONNECTED
    //TRISDbits.RD7 = 1;  // RS-232 RX incoming (in)
    TRISDbits.RD6 = 1;  // IMPROPERLY CONNECTED

    LATA = 0;           // clear flip-flops
    LATC = 0;
    LATD = 0;
    LATE = 0;

    dispSeg(1, '-'); dispSeg(1, '-');
    while (1)
    {
        if (serialRX == 1) dispSeg(1, '1');
        if (serialRX == 0) dispSeg(1, '0');
        else dispSeg(1, '0');
    }

    //while(1) continue;

}

/**
 * Wait for a given number of milliseconds using busy waiting scheme
 * @param time - time in ms to wait
 */
void wait_ms(uint16_t time)
{
    static long timel = 0;
    timel = time * 10; //126l;        // calibrated for 1000 ms ~=1 second
    for (; timel; timel--);
}

/**
 * Display a hexadecimal number on either of the two segment displays
 * @param segment - number identifier of the segment display
 * @param letter - hexadecimal character to display, 0 to turn off
 */
void dispSeg(char segment, char letter)
{
    switch (letter) {
        case 0:             // turn off display
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
        case '0':           // display 0
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = 1;
                seg1g = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = 1;
            seg2g = 0;
            break;
        case '1':           // display 1
            if (segment == 1) {
                seg1b = seg1c = 1;
                seg1a = seg1d = seg1e = seg1f = seg1g = 0;
                return;
            }
            seg2b = seg2c = 1;
            seg2a = seg2d = seg2e = seg2f = seg2g = 0;
            break;
        case '2':           // display 2
            if (segment == 1) {
                seg1a = seg1b = seg1g = seg1e = seg1d = 1;
                seg1c = seg1f = 0;
                return;
            }
            seg2a = seg2b = seg2g = seg2e = seg2d = 1;
            seg2c = seg2f = 0;
            break;
        case '3':           // display 3
            if (segment == 1) {
                seg1a = seg1b = seg1g = seg1c = seg1d = 1;
                seg1e = seg1f = 0;
                return;
            }
            seg2a = seg2b = seg2g = seg2c = seg2d = 1;
            seg2e = seg2f = 0;
            break;
        case '4':           // display 4
            if (segment == 1) {
                seg1f = seg1g = seg1b = seg1c = 1;
                seg1a = seg1d = seg1e = 0;
                return;
            }
            seg2f = seg2g = seg2b = seg2c = 1;
            seg2a = seg2d = seg2e = 0;
            break;
        case '5':           // display 5
            if (segment == 1) {
                seg1a = seg1f = seg1g = seg1c = seg1d = 1;
                seg1b = seg1e = 0;
                return;
            }
            seg2a = seg2f = seg2g = seg2c = seg2d = 1;
            seg2b = seg2e = 0;
            break;
        case '6':           // display 6
            if (segment == 1) {
                seg1a = seg1f = seg1g = seg1c = seg1d = seg1e = 1;
                seg1b = 0;
                return;
            }
            seg2a = seg2f = seg2g = seg2c = seg2d = seg2e = 1;
            seg2b = 0;
            break;
        case '7':           // display 7
            if (segment == 1) {
                seg1a = seg1b = seg1c = 1;
                seg1d = seg1e = seg1f = seg1g = 0;
                return;
            }
            seg2a = seg2b = seg2c = 1;
            seg2d = seg2e = seg2f = seg2g = 0;
            break;
        case '8':           // display 8
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = seg1g = 1;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = seg2g = 1;
            break;
        case '9':           // display 9
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1f = seg1g = 1;
                seg1e = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2f = seg2g = 1;
            seg2e = 0;
            break;
        case 'a' | 'A':     // display A
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1e = seg1f = seg1g = 1;
                seg1d = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2e = seg2f = seg2g = 1;
            seg2d = 0;
            break;
        case 'b' | 'B':     // display B
            if (segment == 1) {
                seg1c = seg1d = seg1e = seg1f = seg1g = 1;
                seg1a = seg1b = 0;
                return;
            }
            seg2c = seg2d = seg2e = seg2f = seg2g = 1;
            seg2a = seg2b = 0;
            break;
        case 'c' | 'C':     // display C
            if (segment == 1) {
                seg1a = seg1d = seg1e = seg1f = 1;
                seg1b = seg1c = seg1g = 0;
                return;
            }
            seg2a = seg2d = seg2e = seg2f = 1;
            seg2b = seg2c = seg2g = 0;
            break;
        case 'd' | 'D':     // display D
            if (segment == 1) {
                seg1b = seg1c = seg1d = seg1e = seg1g = 1;
                seg1a = seg1f = 0;
                return;
            }
            seg2b = seg2c = seg2d = seg2e = seg2g = 1;
            seg2a = seg2f = 0;
            break;
        case 'e' | 'E':     // display E
            if (segment == 1) {
                seg1a = seg1d = seg1e = seg1f = seg1g = 1;
                seg1b = seg1c = 0;
                return;
            }
            seg2a = seg2d = seg2e = seg2f = seg2g = 1;
            seg2b = seg2c = 0;
            break;
        case 'f' | 'F':     // display F
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
 * Cycle through the characters that both segment displays can show
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