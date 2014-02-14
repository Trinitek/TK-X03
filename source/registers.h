/* Register definitions for the Z80 processor emulator */

uint16_t programCounter;            // 16-bit pointer of instruction currently being fetched
uint16_t stackPointer;              // 16-bit pointer of the top of the stack (LIFO organized)

struct indexReg {
    uint8_t regIX;                  // high byte of index register
    uint8_t regIY;                  // low byte of index register
} indexReg;

uint8_t intPageAddress;             // high byte of address in interrupt-based memory jump
uint8_t memoryRefresh;              // memory refresh counter, byte 7 remains static

// main accumulator registers
struct mainAF {
    uint8_t regA;
    uint8_t regF;                   // main flag register
} mainAF;

struct mainBC {
    uint8_t regB;
    uint8_t regC;
} mainBC;

struct mainDE {
    uint8_t regD;
    uint8_t regE;
} mainDE;

struct mainHL {
    uint8_t regH;
    uint8_t regL;
} mainHL;

// alternate accumulator registers
struct altAF {
    uint8_t regA;
    uint8_t regF;                   // alternate flag register
} altAF;

struct altBC {
    uint8_t regB;
    uint8_t regC;
} altBC;

struct altDE {
    uint8_t regD;
    uint8_t regE;
} altDE;

struct altHL {
    uint8_t regH;
    uint8_t regL;
} altHL;