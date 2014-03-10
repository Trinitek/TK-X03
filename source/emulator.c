/* TK3081 machine code processor */

#include <xc.h>                     // XC8 General Include File
#include <stdint.h>                 // For uint8_t definition
#include "registers.h"              // For register, opcode, and memory definitions
#include "ports.c"                  // For virtual port number definitions
#include "serial.h"                 // For UART port and register definitions
#include "segment.h"                // For segment display handling

/*uint16_t regMX_toPointer()
{
    uint16_t pointer = (uint16_t) regMXbits.MH;
    pointer << 8;
    pointer += regMXbits.ML;
    regMX = pointer;
    return pointer;
}/**/


/*void pointerTo_regMX(uint16_t pointer)
{
    uint16_t pointer_lowByte = pointer;
    pointer_lowByte << 8; pointer_lowByte >> 8;
    regMXbits.ML = (uint8_t) pointer_lowByte;
    pointer >> 8;
    regMXbits.MH = (uint8_t) pointer;
}/**/

uint8_t getMXbits(uint8_t subRegister)
{
    uint16_t regMX_temp = regMX;

    if (subRegister == MH)
    {
        regMX_temp >> 8;
    }

    return (uint8_t) regMX_temp;
}

uint16_t immData_toPointer(uint8_t immData_1, uint8_t immData_2)
{
    uint16_t pointer = (uint16_t) immData_1;
    pointer << 8;
    pointer += immData_2;
    return pointer;
}

void initializeRegisters(void)
{
	regA = 0;
	regB = 0;
        pointerTo_regMX(0);
        regPC = 256;					// points to reset vector, bottom of ROM
	regSP = 0;					// points to bottom of stack
	regFbits.CF = regFbits.GF = regFbits.LF = regFbits.OF = regFbits.ZF = 0;
}

/**
 * Import byte from I/O port
 *
 * @param port number
 * @return data from port
 */
uint8_t getPortData(uint8_t portNumber)
{
    switch (portNumber)
    {
        case UART_RCV:
            return rxByte;
    }

    return 0;
}

/**
 * Export byte to I/O port
 *
 * @param port number, data to set
 */
void setPortData(uint8_t portNumber, uint8_t outputData)
{
    uint16_t outputData_temp16;
    switch (portNumber)
    {
        case UART_BAUD:
            if (outputData == 0) outputData_temp16 = 1200;
            if (outputData == 1) outputData_temp16 = 2400;
            if (outputData == 2) outputData_temp16 = 9600;
            if (outputData == 3) outputData_temp16 = 10417;
            if (outputData == 4) outputData_temp16 = 19200;
            enableSerial(outputData_temp16);
            break;

        case UART_SEND:
            txByte = outputData;
            break;

        case SEG_A:
            dispSeg(1, outputData);
            break;

        case SEG_B:
            dispSeg(2, outputData);
            break;
    }
}

void decode_immData(struct immData_t immData)
{
    // loop immData around to readOnly section if arg1 and/or arg2 points outside array boundaries
    if (regPC + 1 = memScratchPad_e + 1) immData.arg1 = memReadOnly;
    if (regPC + 2 = memScratchPad_e + 1) immData.arg2 = memReadOnly;
    if (regPC + 2 = memScratchPad_e + 2) immData.arg2 = memReadOnly + 1;

    // immData.arg1 should never be farther away from the end of memory than memScratchPad_e + 1
}

/**
 * Compare the passed opcode with the instruction set list and do that command
 * 
 * @param opcode, additional 'immediate' data if required
 */
void processOpcode(uint8_t opcode, struct immData_t immData)
{
    // initialize temporary variables
    uint8_t regA_temp8;
    uint16_t regA_temp16;
    uint8_t carriedBit;

    // update combined struct variables and pointers
    regMX_toPointer();
    immData_toPointer(immData);
    
    switch (opcode)
    {
    	/* Processor and flag control */
    
        default:					// do nothing for illegal opcodes and NOP
            break;
            
        case STC:					// set CF
            regFbits.CF = 1;
            break;
            
        case CLC:					// clear CF
            regFbits.CF = 0;
            break;
            
            
        /* Port communications */
            
        case INA:					// A = port [B]
            regA = getPortData(regB);
            break;
        	
        case INI:					// A = port [imm8]
            regA = getPortData(immData.arg1);

            regPC++;
            break;
        	
        case OUTAB:					// port [B] = A
            setPortData(regB, regA);
            break;
        	
        case OUTAI:					// port [imm8] = A
            setPortData(immData.arg1, regA);

            regPC++;
            break;
        	
        case OUTII:					// port [imm8] = [imm8]
            setPortData(immData.arg1, immData.arg2);

            regPC += 2;
            break;
            
            
        /* Data movement instructions */
            
        case MOVAMX:                                    // mov a, [mx]
            regMX_toPointer();

            // does MX point to stack region?
            if (regMX < 256) regA = memoryMap.stack[regMX];
        	
            // does MX point to ROM region?
            if (regMX > 255 && regMX < 512) regA = memoryMap.readOnly[regMX - 256];
            
            // does MX point to scratchPad region?
            if (regMX > 511) regA = memoryMap.scratchPad[regMX - 512];
            
            break;
            
        case MOVAMH:                                    // mov a, mh
            regA = regMXbits.MH;
            break;
            
        case MOVAML:                                    // mov a, ml
            regA = regMXbits.ML;
            break;
            
        case MOVASP:                                    // mov a, sp
            regA = regSP;
            break;
            
        case MOVAF:					// mov a, f
            regA = regF;
            break;
            
        case MOVAB:					// mov a, b
            regA = regB;
            break;
            
        case MOVMXA:                                    // mov [mx], a
            regMX_toPointer();

            // does MX point to stack region?
            if (regMX < 256) memoryMap.stack[regMX] = regA;

            // does MX point to ROM region?
            if (regMX > 255 && regMX < 512) memoryMap.readOnly[regMX - 256] = regA;
            
            // does MX point to scratchPad region?
            if (regMX > 511) memoryMap.scratchPad[regMX - 512] = regA;

            break;
            
        case MOVMHA:                                    // mov mh, a
            regMXbits.MH = regA;
            break;
            
        case MOVMLA:                                    // mov ml, a
            regMXbits.ML = regA;
            break;
            
        case MOVSPA:                                    // mov sp, a
            regSP = regA;
            break;
            
        case MOVBA:					// mov b, a
            regB = regA;
            break;
            
        case MOVI:					// mov a, imm8
            regA = immData.arg1;

            regPC++;
            break;
            
        case XCHG:					// xchg a, b
            regA_temp8 = regA;
            regA = regB;
            regB = regA_temp8;
            break;
            
            
        /* Bitwise operations */
            
        case AND:					// a = a AND b
            regA &= regB;
            break;
        	
        case NAND:					// a = a NAND b
            // inverted AND
            regA = regA & regB;
            regA =~ regA;
            break;
        	
        case OR:					// a = a OR b
            regA |= regB;
            break;
        	
        case NOR:					// a = a NOR b
            // inverted OR
            regA = regA | regB;
            regA =~ regA;
            break;
        	
        case XOR:					// a = a XOR b
            regA ^= regB;
            break;
        	
        case XNOR:					// a = a XNOR b
            // inverted XOR
            regA = regA ^ regB;
            regA =~ regA;
            break;
        	
        case NOT:					// a = NOT a
            regA =~ regA;
            break;
        	
        case SHL:					// shl a, 1
            // determine if a bit will fall off the end
            regA_temp8 = regA;
            regA_temp8 >> 7;
            if (regA_temp8 == 1) regFbits.CF = 1;	// ...set CF if so
            else regFbits.CF = 0;
        	
            // shift bits to the left
            regA << 1;
            break;
        	
        case SHR:					// shr a, 1
            // determine if a bit will fall off the end
            regA_temp8 = regA;
            regA_temp8 << 7;
            regA_temp8 >> 7;
            if (regA_temp8 == 1) regFbits.CF = 1;	// ...set CF if so
            else regFbits.CF = 0;
        	
            // shift A register right 1
            regA >> 1;
            break;
        	
        case ROL:					// rol a, 1
        // preserve leftmost byte
            carriedBit = regA;
            carriedBit >> 7;

            // shift A register left 1
            regA << 1;
        	
            // add carried bit to the end
            regA = regA + carriedBit;
            break;
        	
        case ROR:					// ror a, 1
            // preserve rightmost byte
            carriedBit = regA;
            carriedBit << 7;
        	
            // shift A register right 1
            regA >> 1;
        	
            // add carried bit to the end
            regA += carriedBit;
            break;
        	
        case CMP:					// cmp a, b
            // equal?
            if (regA == regB) regFbits.ZF = 1;
            else regFbits.ZF = 0;
        	
            // less than?
            if (regA < regB) regFbits.LF = 1;
            else regFbits.LF = 0;

            // greater than?
            if (regA > regB) regFbits.GF = 1;
            else regFbits.GF = 0;

            break;
        	
        	
        /* Execution flow operations */
        	
        case JMPMX:					// jump to MX
            regPC = regMX;
            break;
        	
        case JMPI:					// jump to imm16
            regPC = immData_16;

            regPC += 2;
            break;
        	
        case JEMX:					// jump to MX if equal
            if (regFbits.ZF == 1) regPC = regMX;
            break;
        	
        case JEI:					// jump to imm16 if equal
            if (regFbits.ZF == 1) regPC = immData_16;

            regPC += 2;
            break;
        	
        case JNEMX:					// jump to MX if not equal
            if (regFbits.ZF == 0) regPC = regMX;
            break;
        	
        case JNEI:					// jump to imm16 if not equal
            if (regFbits.ZF == 0) regPC = immData_16;

            regPC += 2;
            break;
        	
        case JGMX:					// jump to MX if greater than
            if (regFbits.GF == 1) regPC = regMX;
            break;

        case JGI:					// jump to imm16 if greater than
            if (regFbits.GF == 1) regPC = immData_16;

            regPC += 2;
            break;
        	
        case JLMX:					// jump to MX if less than
            if (regFbits.LF == 1) regPC = regMX;
            break;
        	
        case JLI:					// jump to imm16 if less than
            if (regFbits.LF == 1) regPC = immData_16;

            regPC += 2;
            break;
        	
        case JCMX:					// jump to MX if CF set
            if (regFbits.CF == 1) regPC = regMX;
            break;
        	
        case JCI:					// jump to imm16 if CF set
            if (regFbits.CF == 1) regPC = immData_16;

            regPC += 2;
            break;
        	
        case JNCMX:					// jump to MX if CF not set
            if (regFbits.CF == 0) regPC = regMX;
            break;
        	
        case JNCI:					// jump to imm16 if CF not set
            if (regFbits.CF == 0) regPC = immData_16;

            regPC += 2;
            break;
        	
        case CALLMX:                                    // push PC to stack and jump to MX
            memoryMap.stack[regSP] = regPC;
            regSP += 2;
            regPC = regMX;
            break;
        	
        case CALLI:					// push PC to stack and jump to imm16
            memoryMap.stack[regSP] = regPC;
            regSP += 2;
            regPC = immData_16;

            regPC += 2;
            break;
        	
        case RET:					// pop PC from stack and return
            regSP -= 2;
            regPC = memoryMap.stack[regSP];
            break;
        	
        	
        /* Stack operations */
        	
        case PUSHA:					// push a
            memoryMap.stack[regSP] = regA;
            regSP++;
            break;
        	
        case PUSHB:					// push b
            memoryMap.stack[regSP] = regB;
            regSP++;
            break;
        	
        case PUSHMX:                                    // push mx
            memoryMap.stack[regSP] = regMX;
            regSP += 2;
            break;
        	
        case PUSHMH:                                    // push mh
            memoryMap.stack[regSP] = regMXbits.MH;
            regSP++;
            break;
        	
        case PUSHML:                                    // push ml
            memoryMap.stack[regSP] = regMXbits.ML;
            regSP++;
            break;
        	
        case PUSHF:					// push f
            memoryMap.stack[regSP] = regF;
            regSP++;
            break;
        	
        case PUSHI:					// push imm8
            memoryMap.stack[regSP] = immData.arg1;
            regSP++;

            regPC++;
            break;
        	
        case POPA:					// pop a
            regSP--;
            regA = memoryMap.stack[regSP];
            break;
        	
        case POPB:					// pop b
            regSP--;
            regB = memoryMap.stack[regSP];
            break;
        	
        case POPMX:					// pop mx
            regSP -= 2;
            regMX = memoryMap.stack[regSP];
            break;
        	
        case POPMH:					// pop mh
            regSP--;
            regMXbits.MH = memoryMap.stack[regSP];
            break;
        	
        case POPML:					// pop ml
            regSP--;
            regMXbits.ML = memoryMap.stack[regSP];
            break;
        	
        	
        /* Mathematical operations */
        	
        case ADDA:                                      // a = a + b
            // check for potential overflow and set corresponding flag
            regA_temp16 += regB;
            if (regA_temp16 > 255) regFbits.OF = 1;
            else regFbits.OF = 0;

            regA += regB;
            break;
        	
        case ADDI:					// a = a + imm8
           // check for potential overflow and set corresponding flag
           regA_temp16 += immData.arg1;
           if (regA_temp16 > 255) regFbits.OF = 1;
           else regFbits.OF = 0;

            regA += immData.arg1;

            regPC++;
            break;
        	
        case SUBA:					// a = a - b
            // check for potential overflow and set corresponding flag
            regA_temp16 = regA;
            regA_temp16 << 8;
            regA_temp16 += 255;
            regA_temp16 -= regB;
            if (regA_temp16 < 255) regFbits.OF = 1;
            else regFbits.OF = 0;

            regA -= regB;
            break;
        	
        case SUBI:					// a = a - imm8
            // check for potential overflow and set corresponding flag
            regA_temp16 = regA;
            regA_temp16 << 8;
            regA_temp16 += 255;
            regA_temp16 -= immData.arg1;
            if (regA_temp16 < 255) regFbits.OF = 1;
            else regFbits.OF = 0;

            regA -= immData.arg1;

            regPC++;
            break;
        	
        case INC:					// inc a
            // check for potential overflow and set corresponding flag
            regA_temp16 = regA + 1;
            if (regA_temp16 > 255) regFbits.OF = 1;
            else regFbits.OF = 0;

            regA++;
            break;
        	
        case DEC:					// dec a
            // check for potential overflow and set corresponding flag
            regA_temp8 = regA - 1;
            if (regA_temp8 == 255) regFbits.OF = 1;
            else regFbits.OF = 0;

            regA--;
            break;
    }

    regPC++;                                            // increment regPC to point to next opcode; opcodes that need additional data increment PC accordingly
}