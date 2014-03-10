/* TK3081 machine code processor */
#include "registers.h"

uint16_t regMX_toPointer();
void pointerTo_regMX(uint16_t pointer);
uint16_t immData_toPointer(uint8_t immData_1, uint8_t immData_2);

uint8_t getPortData(uint8_t portNumber);
void setPortData(uint8_t portNumber, uint8_t outputData);

void initializeRegisters(void);
void decode_immData(uint8_t immData_1, uint8_t immData_2);
void processOpcode(uint8_t opcode, uint8_t immData_1, uint8_t immData_2);