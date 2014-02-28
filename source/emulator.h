/* Z80 machine code processor */
#include "registers.c"

uint16_t regMX_toPointer();
void pointerTo_regMX(uint16_t pointer);
uint16_t immData_toPointer(struct immData_t immData);

uint8_t getPortData(uint8_t portNumber);
void setPortData(uint8_t portNumber, uint8_t outputData);

void initializeRegisters(void);
void decode_immData(struct immData_t immData);
void processOpcode(uint8_t opcode, struct immData_t immData);