/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef XC_PIN_H
#define	XC_PIN_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    volatile uint8_t * port;
    volatile uint8_t * ddr;
    volatile uint8_t * pin;
    uint8_t index;
} GPIO;

typedef enum {
    TRISTATE,
    INPUT_SOURCING,
    OUTPUT_SINK,
    OUTPUT_SOURCE
} GPIOState;

extern GPIO CCD1;
extern GPIO CCD2;
extern GPIO CCD3;
extern GPIO CCD4;
extern GPIO CCL12;
extern GPIO CCL3;
extern GPIO AAL123;
extern GPIO AB;
extern GPIO AC;
extern GPIO AD;
extern GPIO AE;
extern GPIO AF;
extern GPIO AG;
extern GPIO ADP;

extern GPIO SW1;
extern GPIO SW2;
extern GPIO SW3;
extern GPIO SW4;
extern GPIO SW5;
extern GPIO* SW[5];

void setRegBit(volatile uint8_t* reg, uint8_t pin, bool state);
void setGPIO(GPIO pin, GPIOState state);
bool readGPIO(GPIO pin);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_PIN_H */
