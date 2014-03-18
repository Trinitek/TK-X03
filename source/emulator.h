/* TK3081 machine code processor */
//#include "registers.h"

uint8_t getMXbits(uint8_t);
void setMXbits(uint8_t subRegister, uint8_t value);
uint8_t getFbits(uint8_t);
void setFbits(uint8_t subRegister, uint8_t value);

uint16_t immData_toPointer(void);

uint8_t getPortData(uint8_t);
void setPortData(uint8_t portNumber, uint8_t outputData);

void initializeRegisters(void);
void update_immData(void);
void processOpcode(void);