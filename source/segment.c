/* Segment display handling */

#include <stdint.h>                 // For uint8_t definition
#include "configuration_bits.c"     // For pin definitions
#include "system.h"                 // for wait_ms function

/**
 * Display a hexadecimal number on either of the two segment displays.
 *
 * @param segment - number identifier of the segment display
 * @param letter - hexadecimal character to display, 0 or 'x' to turn off
 */
void dispSeg(uint8_t segment, uint8_t letter)
{
    switch (letter) {
        case 'x':
        case 'X':     // turn off display
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = seg1g = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2d = seg2e = seg2f = seg2g = 0;
            break;
        case '-':               // display dash
            if (segment == 1) {
                seg1g = 1;
                seg1a = seg1b = seg1c = seg1d = seg1e = seg1f = 0;
                return;
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
        case 'a':
        case 'A':               // display A
            if (segment == 1) {
                seg1a = seg1b = seg1c = seg1e = seg1f = seg1g = 1;
                seg1d = 0;
                return;
            }
            seg2a = seg2b = seg2c = seg2e = seg2f = seg2g = 1;
            seg2d = 0;
            break;
        case 'b':
        case 'B':               // display B
            if (segment == 1) {
                seg1c = seg1d = seg1e = seg1f = seg1g = 1;
                seg1a = seg1b = 0;
                return;
            }
            seg2c = seg2d = seg2e = seg2f = seg2g = 1;
            seg2a = seg2b = 0;
            break;
        case 'c':
        case 'C':               // display C
            if (segment == 1) {
                seg1a = seg1d = seg1e = seg1f = 1;
                seg1b = seg1c = seg1g = 0;
                return;
            }
            seg2a = seg2d = seg2e = seg2f = 1;
            seg2b = seg2c = seg2g = 0;
            break;
        case 'd':
        case 'D':               // display D
            if (segment == 1) {
                seg1b = seg1c = seg1d = seg1e = seg1g = 1;
                seg1a = seg1f = 0;
                return;
            }
            seg2b = seg2c = seg2d = seg2e = seg2g = 1;
            seg2a = seg2f = 0;
            break;
        case 'e':
        case 'E':               // display E
            if (segment == 1) {
                seg1a = seg1d = seg1e = seg1f = seg1g = 1;
                seg1b = seg1c = 0;
                return;
            }
            seg2a = seg2d = seg2e = seg2f = seg2g = 1;
            seg2b = seg2c = 0;
            break;
        case 'f':
        case 'F':               // display F
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