/* Register definitions for the TK3081 processor emulator */
#include <stdint.h>

// global variable -- should be redefined only once elsewhere without "extern"
extern uint8_t haltFlag;

uint8_t regA;                               // 8-bit primary accumulator
uint8_t regB;                               // 8-bit secondary accumulator

uint16_t regMX;                             // 16-bit general use pointer
#define MH              0                   // ...high byte
#define ML              1                   // ...low byte

uint16_t regPC;                             // 16-bit pointer of instruction currently being fetched

uint8_t regSP;                              // 8-bit pointer of the top of the stack (LIFO organized)

uint8_t regF;                               // 8-bit flag register
#define CF              0                   // ...carry
#define GF              1                   // ...greater than
#define LF              2                   // ...less than
#define ZF              3                   // ...zero/unequal
#define OF              4                   // ...overflow

/* Temporary data and memory map */

uint8_t immData_1;                          // temp storage of the imm# data required by some instructions
uint8_t immData_2;

#define memScratchPad_e 2047                // last byte offset in memory, available memory is 2048 bytes

uint8_t stack[256];                         // stack space that is separate from the memory space

/* // INSTRUCTION SET // */

/* Processor and flag control */

#define NOPI 		0x00                // do nothing, "I" postfix to prevent conflicts with pic18 header file
#define STC  		0x01                // set CF
#define CLC  		0x02                // clear CF

#define HLT             0x08                // stop the processor

/* Port communications */

#define INA		0x03                // set A to data on port specified by reg B
#define INI		0x04                // set A to data on port specified by imm8
#define OUTAB		0x05                // set port specified by B to A
#define OUTAI		0x06                // set port specified by imm8 to A
#define OUTII		0x07                // set port specified by imm8 to imm8

/* Data movement instructions */

#define MOVAMX  	0x10                // set A to [MX]
#define MOVAMH   	0x11                // set A to MH
#define MOVAML   	0x12                // set A to ML
#define MOVASP   	0x13                // set A to SP
#define MOVAF   	0x14                // set A to F
#define MOVAB   	0x15                // set A to B

#define MOVMXA   	0x16                // set [MX] to A
#define MOVMHA   	0x17                // set MH to A
#define MOVMLA   	0x18                // set ML to A
#define MOVSPA   	0x19                // set SH to A
#define MOVBA   	0x1A                // set B to A

#define MOVI   		0x1B                // set A to imm8
#define XCHG   		0x1C                // swap A and B data


/* Bitwise operations */

#define AND   		0x20                // A = A and B
#define NAND   		0x21                // A = A nand B
#define OR   		0x22                // A = A or B
#define NOR   		0x23                // A = A nor B
#define XNOR   		0x24                // A = A xnor B
#define NOT   		0x25                // A = not A

#define SHL   		0x26                // A = shift left by 1, set carry
#define SHR   		0x27                // A = shift right by 1, set carry
#define ROL   		0x28                // A = rotate left by 1
#define ROR   		0x29                // A = rotate right by 1

#define CMP             0x2A                // compare A to B, set flag registers

#define XOR		0x2B                // A = A xor B


/* Execution flow operations */

#define JMPMX   	0x30                // jump to MX
#define JMPI   		0x31                // jump to imm16
#define JEMX   		0x32                // jump to MX if ZF set
#define JEI   		0x33                // jump to imm16 if ZF set
#define JNEMX   	0x34                // jump to MX if ZF clear
#define JNEI   		0x35                // jump to imm16 if ZF clear
#define JGMX   		0x36                // jump to MX if GF set
#define JGI   		0x37                // jump to imm16 if GF set
#define JLMX   		0x38                // jump to MX if LF set
#define JLI   		0x39                // jump to imm16 if LF set
#define JCMX   		0x3A                // jump to MX if CF set
#define JCI   		0x3B                // jump to imm16 if CF set
#define JNCMX   	0x3C                // jump to MX if CF clear
#define JNCI   		0x3D                // jump to imm16 if CF clear

#define CALLMX   	0x3E                // PUSHPC, JMPMX
#define CALLI   	0x3F                // PUSHPC, JMPI aaaaaaaa_aaaaaaaa

// **
#define RET   		0x40                // POPPC, JMP


/* Stack operations */

#define PUSHA   	0x41                // push A to stack, SP+1
#define PUSHB   	0x42                // push B to stack, SP+1
#define PUSHMX   	0x43                // push MX to stack, SP+2
#define PUSHMH   	0x44                // push MH to stack, SP+1
#define PUSHML   	0x45                // push ML to stack, SP+1
#define PUSHF  		0x46                // push F to stack, SP+1
#define PUSHI   	0x47                // push imm8 to stack, SP+1

#define POPA   		0x48                // pop A from stack, SP-1
#define POPB   		0x49                // pop B from stack, SP-1
#define POPMX   	0x4A                // pop MX from stack, SP-2
#define POPMH   	0x4B                // pop MH from stack, SP-1
#define POPML   	0x4C                // pop ML from stack, SP-1


/* Mathematical operations */

#define ADDA   		0x50                // A = A + B
#define ADDI   		0x51                // A = A + imm8
#define SUBA   		0x52                // A = A - B
#define SUBI   		0x53                // A = A - imm8
#define INC		0x54                // A++
#define DEC		0x55                // A--