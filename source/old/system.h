/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        16000000L
#define FCY             SYS_FREQ ///4

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
void wait_ms(uint16_t);
