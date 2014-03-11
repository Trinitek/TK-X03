/* TK3081 machine code processor */
//#include "registers.h"

//uint16_t regMX_toPointer();
//void pointerTo_regMX(uint16_t pointer);

uint8_t getMXbits(uint8_t);
void setMXbits(uint8_t, uint8_t);
uint8_t getFbits(uint8_t);
void setFbits(uint8_t, uint8_t);

uint16_t immData_toPointer(void);

uint8_t getPortData(uint8_t);
void setPortData(uint8_t, uint8_t);

void initializeRegisters(void);
void update_immData(void);
void processOpcode(void);