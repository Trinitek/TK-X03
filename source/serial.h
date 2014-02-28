/* RS-232 serial interface control */

void enableSerial(uint16_t);


/* Pin and register definitions for the RS-232 interface controller and UART */

#define serialEn    LATDbits.LD3    // enable controller
#define serialTX    LATDbits.LD6    // TX outgoing
#define serialRX    PORTDbits.RD7   // RX incoming
#define serialREQ   LATDbits.LD5    // request to send
#define serialOK    PORTDbits.RD4   // clear to send

#define txByte      TXREG2          // buffer for characters to be sent
#define rxByte      RCREG2          // buffer for received character