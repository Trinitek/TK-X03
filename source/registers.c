/* Register definitions for the TK3081 processor emulator */
#include <stdint.h>

uint8_t regA;                               // 8-bit primary accumulator
uint8_t regB;                               // 8-bit secondary accumulator


struct {                                    // 16-bit general use pointer
        uint8_t MH;
        uint8_t ML;
} regMXbits; uint16_t regMX;

uint16_t regPC;                             // 16-bit pointer of instruction currently being fetched

uint8_t regSP;                              // 8-bit pointer of the top of the stack (LIFO organized)

struct {
        uint8_t CF    :1;                   // carry flag
        uint8_t GF    :1;                   // greater than
        uint8_t LF    :1;                   // less than
        uint8_t ZF    :1;                   // zero/equal flag
        uint8_t OF    :1;                   // overflow flag
        uint8_t       :3;                   // unimplemented, set to 0
} regFbits; uint8_t regF;

/* Temporary data and memory map */

struct immData_t {                          // temp storage of the imm# data required by some opcodes
	uint8_t arg1;
	uint8_t arg2;
}; uint16_t immData_16;

uint8_t virtualMemory[2048];
#define memStack        0                   // stack space 0-255 (256 bytes)
#define memStack_e      255
#define memReadOnly     256                 // read only memory 256-511 (256 bytes) - reset vector at base
#define memReadOnly_e   511
#define memScratchPad   512                 // scratchpad space 512-2047 (1536 bytes) - usable by program
#define memScratchPad_e 2047

/* Processor and flag control */

#define NOPI 		0b00000000          // do nothing, "I" postfix to prevent conflicts with pic18 header file
#define STC  		0b00000001          // set CF
#define CLC  		0b00000010          // clear CF

/* Port communications */

#define INA		0b00000011          // set A to data on port specified by reg B
#define INI		0b00000100          // set A to data on port specified by imm8
#define OUTAB		0b00000101          // set port specified by B to A
#define OUTAI		0b00000110          // set port specified by imm8 to A
#define OUTII		0b00000111          // set port specified by imm8 to imm8

/* Data movement instructions */

#define MOVAMX  	0b00010000          // set A to [MX]
#define MOVAMH   	0b00010001          // set A to MH
#define MOVAML   	0b00010010          // set A to ML
#define MOVASP   	0b00010011          // set A to SP
#define MOVAF   	0b00010100          // set A to F
#define MOVAB   	0b00010101          // set A to B

#define MOVMXA   	0b00010110          // set [MX] to A
#define MOVMHA   	0b00010111          // set MH to A
#define MOVMLA   	0b00011000          // set ML to A
#define MOVSPA   	0b00011001          // set SH to A
#define MOVBA   	0b00011010          // set B to A

#define MOVI   		0b00011011          // set A to imm8
#define XCHG   		0b00011100          // swap A and B data


/* Bitwise operations */

#define AND   		0b00100000          // A = A and B
#define NAND   		0b00100001          // A = A nand B
#define OR   		0b00100010          // A = A or B
#define NOR   		0b00100011          // A = A nor B
#define XNOR   		0b00100100          // A = A xnor B
#define NOT   		0b00100101          // A = not A

#define SHL   		0b00100110          // A = shift left by 1, set carry
#define SHR   		0b00100111          // A = shift right by 1, set carry
#define ROL   		0b00101000          // A = rotate left by 1
#define ROR   		0b00101001          // A = rotate right by 1

#define CMP             0b00101010          // compare A to B, set flag registers

#define XOR		0b00101011          // A = A xor B


/* Execution flow operations */

#define JMPMX   	0b00110000          // jump to MX
#define JMPI   		0b00110001          // jump to imm16
#define JEMX   		0b00110010          // jump to MX if ZF set
#define JEI   		0b00110011          // jump to imm16 if ZF set
#define JNEMX   	0b00110100          // jump to MX if ZF clear
#define JNEI   		0b00110101          // jump to imm16 if ZF clear
#define JGMX   		0b00110110          // jump to MX if GF set
#define JGI   		0b00110111          // jump to imm16 if GF set
#define JLMX   		0b00111000          // jump to MX if LF set
#define JLI   		0b00111001          // jump to imm16 if LF set
#define JCMX   		0b00111010          // jump to MX if CF set
#define JCI   		0b00111011          // jump to imm16 if CF set
#define JNCMX   	0b00111100          // jump to MX if CF clear
#define JNCI   		0b00111101          // jump to imm16 if CF clear

#define CALLMX   	0b00111110          // PUSHPC, JMPMX
#define CALLI   	0b00111111          // PUSHPC, JMPI aaaaaaaa_aaaaaaaa

// **
#define RET   		0b01000000          // POPPC, JMP


/* Stack operations */

#define PUSHA   	0b01000001          // push A to stack, SP+1
#define PUSHB   	0b01000010          // push B to stack, SP+1
#define PUSHMX   	0b01000011          // push MX to stack, SP+2
#define PUSHMH   	0b01000100          // push MH to stack, SP+1
#define PUSHML   	0b01000101          // push ML to stack, SP+1
#define PUSHF  		0b01000110          // push F to stack, SP+1
#define PUSHI   	0b01000111          // push imm8 to stack, SP+1

#define POPA   		0b01101000          // pop A from stack, SP-1
#define POPB   		0b01101001          // pop B from stack, SP-1
#define POPMX   	0b01101010          // pop MX from stack, SP-2
#define POPMH   	0b01101011          // pop MH from stack, SP-1
#define POPML   	0b01101100          // pop ML from stack, SP-1


/* Mathematical operations */

#define ADDA   		0b01010000          // A = A + B
#define ADDI   		0b01010001          // A = A + imm8
#define SUBA   		0b01010010          // A = A - B
#define SUBI   		0b01010011          // A = A - imm8
#define INC		0b01010100          // A++
#define DEC		0b01010101          // A--