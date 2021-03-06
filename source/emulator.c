/* TK3081 machine code processor */

#include <xc.h>                     // XC8 General Include File
#include <stdint.h>                 // For uint8_t definition
#include "system.h"                 // For hexToChar and timing facilities
#include "registers.h"              // For register and opcode definitions
#include "ports.h"                  // For virtual port number definitions
#include "serial.h"                 // For UART port and register definitions
#include "segment.h"                // For segment display handling
#include "virtualMemory.h"          // For memory definition

uint8_t haltFlag = 0;

/**
 * Get the value of the subregister specified
 * 
 * @param subregister
 * @return value
 */
uint8_t getMXbits(uint8_t subRegister)
{
    uint16_t regMX_temp = regMX;

    if (subRegister == MH)
    {
        regMX_temp >>= 8;
    }

    return (uint8_t) regMX_temp;
}

/**
 * Set the subregister specified to the value specified
 * 
 * @param subRegister, value
 */
void setMXbits(uint8_t subRegister, uint8_t value)
{
    uint16_t value_temp = (uint16_t) value;
    uint16_t regMX_temp = regMX;
    
    switch(subRegister)
    {
        case MH:
            regMX_temp <<= 8;
            regMX_temp >>= 8;
            value_temp <<= 8;
            break;
        
        case ML:
            regMX_temp >>= 8;
            regMX_temp <<= 8;
            break;
    }
    
    regMX = regMX_temp + value_temp;
}

/**
 * Get the value of the flag specified
 * 
 * @param flag
 * @return value
 */
uint8_t getFbits(uint8_t subRegister)
{
    uint8_t regF_temp = regF;
    
    switch(subRegister)
    {
        case CF:
            regF_temp <<= 7;
            regF_temp >>= 7;
            break;
            
        case GF:
            regF_temp <<= 6;
            regF_temp >>= 7;
            break;
            
        case LF:
            regF_temp <<= 5;
            regF_temp >>= 7;
            break;
            
        case ZF:
            regF_temp <<= 4;
            regF_temp >>= 7;
            break;
    }
    
    return regF_temp;
}

/**
 * Set the flag specified to the value specified
 * 
 * @param flag, value
 */
void setFbits(uint8_t subRegister, uint8_t value)
{
    uint8_t regF_temp1 = regF;
    uint8_t regF_temp2 = regF;
    
    switch(subRegister)
    {
        case CF:
            regF_temp1 >>= 1;
            regF_temp1 <<= 1;
            regF_temp2 = 0;
            break;
            
        case GF:
            regF_temp1 >>= 2;
            regF_temp1 <<= 2;
            regF_temp2 <<= 6;
            regF_temp2 >>= 6;
            break;
            
        case LF:
            regF_temp1 >>= 3;
            regF_temp1 <<= 3;
            regF_temp2 <<= 5;
            regF_temp2 >>= 5;
            break;
            
        case ZF:
            regF_temp1 >>= 4;
            regF_temp1 <<= 4;
            regF_temp2 <<= 4;
            regF_temp2 >>= 4;
            break;
    }
    
    regF = regF_temp1 + regF_temp2 + value;
}

uint16_t immData_toPointer(void)
{
    uint16_t pointer = (uint16_t) immData_1;
    pointer <<= 8;
    pointer += immData_2;
    return pointer;
}

void initializeRegisters(void)
{
        haltFlag = 0;
	regA = 0;
	regB = 0;
        regPC = 0;					// points to reset vector, bottom of ROM
	regSP = 0;					// points to bottom of stack
        regF = 0;
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
    uint8_t outputData_temp8;
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
            outputData_temp8 = hexToChar(outputData);
            dispSeg(1, outputData_temp8);
            break;

        case SEG_B:
            outputData_temp8 = hexToChar(outputData);
            dispSeg(2, outputData_temp8);
            break;
    }
}

void update_immData(void)
{
    // loop immData around to readOnly section if arg1 and/or arg2 points outside array boundaries
    if (regPC + 1 == memScratchPad_e + 1) immData_1 = 0;
    if (regPC + 2 == memScratchPad_e + 1) immData_2 = 0;
    if (regPC + 2 == memScratchPad_e + 2) immData_2 = 1;

    // immData.arg1 should never be farther away from the end of memory than memScratchPad_e + 1

    immData_1 = virtualMemory[regPC + 1];
    immData_2 = virtualMemory[regPC + 2];
}

/**
 * Compare the passed opcode with the instruction set list and do that command
 * Uses global regPC, immData_1, and immData_2 variables!
 */
void processOpcode(void)
{
    // update immdediate data variables for the next iteration
    update_immData();
    
    // initialize temporary variables
    uint8_t regA_temp8 = regA;
    uint16_t regA_temp16 = (uint16_t) regA;
    uint16_t regPC_temp = regPC;
    uint8_t carriedBit;

    // decode opcode and execute the associated command
    switch (virtualMemory[regPC])
    {
    	/* Processor and flag control */
    
        default:					// do nothing for illegal opcodes and NOP
            break;
            
        case STC:					// set CF
            setFbits(CF, 1);
            break;
            
        case CLC:					// clear CF
            setFbits(CF, 0);
            break;

        case HLT:
            haltFlag = 1;                               // set internal halt flag to true
            break;
            
            
        /* Port communications */
            
        case INA:					// A = port [B]
            regA = getPortData(regB);
            break;
        	
        case INI:					// A = port [imm8]
            regA = getPortData(immData_1);
            regPC++;
            break;
        	
        case OUTAB:					// port [B] = A
            setPortData(regB, regA);
            break;
        	
        case OUTAI:					// port [imm8] = A
            setPortData(immData_1, regA);
            regPC++;
            break;
        	
        case OUTII:					// port [imm8] = [imm8]
            setPortData(immData_1, immData_2);
            regPC += 2;
            break;
            
            
        /* Data movement instructions */
            
        case MOVAMX:                                    // mov a, [mx]
            regA = virtualMemory[regMX];
            break;
            
        case MOVAMH:                                    // mov a, mh
            regA = getMXbits(MH);
            break;
            
        case MOVAML:                                    // mov a, ml
            regA = getMXbits(ML);
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
            // does MX point to stack region?
            //if (regMX < 256) memoryMap.stack[regMX] = regA;

            // does MX point to ROM region?
            //if (regMX > 255 && regMX < 512) memoryMap.readOnly[regMX - 256] = regA;
            
            // does MX point to scratchPad region?
            //if (regMX > 511) memoryMap.scratchPad[regMX - 512] = regA;

            virtualMemory[regMX] = regA;

            break;
            
        case MOVMHA:                                    // mov mh, a
            setMXbits(MH, regA);
            break;
            
        case MOVMLA:                                    // mov ml, a
            setMXbits(ML, regA);
            break;
            
        case MOVSPA:                                    // mov sp, a
            regSP = regA;
            break;
            
        case MOVBA:					// mov b, a
            regB = regA;
            break;
            
        case MOVI:					// mov a, imm8
            regA = immData_1;

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
            regA_temp8 >>= 7;
            if (regA_temp8 == 1) setFbits(CF, 1);	// ...set CF if so
            else setFbits(CF, 0);
        	
            // shift bits to the left
            regA <<= 1;
            break;
        	
        case SHR:					// shr a, 1
            // determine if a bit will fall off the end
            regA_temp8 = regA;
            regA_temp8 <<= 7;
            regA_temp8 >>= 7;
            if (regA_temp8 == 1) setFbits(CF, 1);	// ...set CF if so
            else setFbits(CF, 0);
        	
            // shift A register right 1
            regA >>= 1;
            break;
        	
        case ROL:					// rol a, 1
            // preserve leftmost byte
            carriedBit = regA;
            carriedBit >>= 7;

            // shift A register left 1
            regA <<= 1;
        	
            // add carried bit to the end
            regA = regA + carriedBit;
            break;
        	
        case ROR:					// ror a, 1
            // preserve rightmost byte
            carriedBit = regA;
            carriedBit <<= 7;
        	
            // shift A register right 1
            regA >>= 1;
        	
            // add carried bit to the end
            regA += carriedBit;
            break;
        	
        case CMP:					// cmp a, b
            // equal?
            if (regA == regB) setFbits(ZF, 1);
            else setFbits(ZF, 0);
        	
            // less than?
            if (regA < regB) setFbits(LF, 1);
            else setFbits(LF, 0);

            // greater than?
            if (regA > regB) setFbits(GF, 1);
            else setFbits(GF, 0);

            break;
        	
        	
        /* Execution flow operations */
        	
        case JMPMX:					// jump to MX
            regPC = regMX;
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JMPI:					// jump to imm16
            regPC = immData_toPointer();
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JEMX:					// jump to MX if equal
            if (getFbits(ZF) == 1) regPC = regMX;
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JEI:					// jump to imm16 if equal
            if (getFbits(ZF) == 1) regPC = immData_toPointer();
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JNEMX:					// jump to MX if not equal
            if (getFbits(ZF) == 0) regPC = regMX;
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JNEI:					// jump to imm16 if not equal
            if (getFbits(ZF) == 0) regPC = immData_toPointer();
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JGMX:					// jump to MX if greater than
            if (getFbits(GF) == 1) regPC = regMX;
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;

        case JGI:					// jump to imm16 if greater than
            if (getFbits(GF) == 1) regPC = immData_toPointer();
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JLMX:					// jump to MX if less than
            if (getFbits(LF) == 1) regPC = regMX;
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JLI:					// jump to imm16 if less than
            if (getFbits(LF) == 1) regPC = immData_toPointer();
            regPC += 2;
            break;
        	
        case JCMX:					// jump to MX if CF set
            if (getFbits(CF) == 1) regPC = regMX;
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JCI:					// jump to imm16 if CF set
            if (getFbits(CF) == 1) regPC = immData_toPointer();
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JNCMX:					// jump to MX if CF not set
            if (getFbits(CF) == 0) regPC = regMX;
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case JNCI:					// jump to imm16 if CF not set
            if (getFbits(CF) == 0) regPC = immData_toPointer();
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case CALLMX:                                    // push PC to stack and jump to MX
            // point to next instruction before storing regPC
            regPC++;
            regPC_temp = regPC;
            regPC_temp >>= 8;
            stack[regSP] = (uint8_t) regPC_temp;
            regSP++;
            stack[regSP] = (uint8_t) regPC;
            regSP++;
            regPC = regMX;
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case CALLI:					// push PC to stack and jump to imm16
            // point to next instruction before storing regPC
            regPC += 3;
            regPC_temp = regPC;
            regPC_temp >>= 8;
            stack[regSP] = (uint8_t) regPC_temp;
            regSP++;
            stack[regSP] = (uint8_t) regPC;
            regSP++;
            regPC = immData_toPointer();
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        case RET:					// pop PC from stack and return
            regSP--;
            regPC = (uint16_t) stack[regSP];
            regSP--;
            regPC_temp = (uint16_t) stack[regSP];
            regPC_temp <<= 8;
            regPC += regPC_temp;
            // offset regPC++ at the end of the switch/case
            regPC--;
            break;
        	
        	
        /* Stack operations */
        	
        case PUSHA:					// push a
            stack[regSP] = regA;
            regSP++;
            break;
        	
        case PUSHB:					// push b
            stack[regSP] = regB;
            regSP++;
            break;
        	
        case PUSHMX:                                    // push mx
            stack[regSP] = regMX;
            regSP += 2;
            break;
        	
        case PUSHMH:                                    // push mh
            stack[regSP] = getMXbits(MH);
            regSP++;
            break;
        	
        case PUSHML:                                    // push ml
            stack[regSP] = getMXbits(ML);
            regSP++;
            break;
        	
        case PUSHF:					// push f
            stack[regSP] = regF;
            regSP++;
            break;
        	
        case PUSHI:					// push imm8
            stack[regSP] = immData_1;
            regSP++;
            regPC++;
            break;
        	
        case POPA:					// pop a
            regSP--;
            regA = stack[regSP];
            break;
        	
        case POPB:					// pop b
            regSP--;
            regB = stack[regSP];
            break;
        	
        case POPMX:					// pop mx
            regSP -= 2;
            regMX = stack[regSP];
            break;
        	
        case POPMH:					// pop mh
            regSP--;
            setMXbits(MH, stack[regSP]);
            break;
        	
        case POPML:					// pop ml
            regSP--;
            setMXbits(ML, stack[regSP]);
            break;
        	
        	
        /* Mathematical operations */
        	
        case ADDA:                                      // a = a + b
            // check for potential overflow and set corresponding flag
            regA_temp16 += (uint16_t) regB;
            if (regA_temp16 > 255) setFbits(CF, 1);
            else setFbits(CF, 0);
            regA += regB;
            break;
        	
        case ADDI:					// a = a + imm8
           // check for potential overflow and set corresponding flag
           regA_temp16 += (uint16_t) immData_1;
           if (regA_temp16 > 255) setFbits(CF, 1);
           else setFbits(CF, 0);

           regA += immData_1;

           regPC++;
           break;
        	
        case SUBA:					// a = a - b
           // check for potential overflow and set corresponding flag
           regA_temp16 = regA;
           regA_temp16 <<= 8;
           regA_temp16 += 255;
           regA_temp16 -= regB;
           if (regA_temp16 < 255) setFbits(CF, 1);
           else setFbits(CF, 0);
           regA -= regB;
           break;
        	
        case SUBI:					// a = a - imm8
           // check for potential overflow and set corresponding flag
           regA_temp16 = regA;
           regA_temp16 <<= 8;
           regA_temp16 += 255;
           regA_temp16 -= immData_1;
           if (regA_temp16 < 255) setFbits(CF, 1);
           else setFbits(CF, 0);
           regA -= immData_1;
           regPC++;
           break;
        	
        case INC:					// inc a
           // check for potential overflow and set corresponding flag
           regA_temp16 = regA + 1;
           if (regA_temp16 > 255) setFbits(CF, 1);
           else setFbits(CF, 0);
           regA++;
           break;
        	
        case DEC:					// dec a
           // check for potential overflow and set corresponding flag
           regA_temp8 = regA - 1;
           if (regA_temp8 == 255) setFbits(CF, 1);
           else setFbits(CF, 0);
           regA--;
           break;
    }

    // increment regPC to point to next opcode; opcodes that need additional data increment PC accordingly
    regPC++; 
}
