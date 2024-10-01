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
    uint8_t pin;
} Pin;

typedef enum {
    TRISTATE,
    INPUT_SOURCING,
    OUTPUT_SINK,
    OUTPUT_SOURCE
} PinState;

extern Pin CCD1;
extern Pin CCD2;
extern Pin CCD3;
extern Pin CCD4;
extern Pin CCL12;
extern Pin CCL3;
extern Pin AAL123;
extern Pin AB;
extern Pin AC;
extern Pin AD;
extern Pin AE;
extern Pin AF;
extern Pin AG;
extern Pin ADP;

void setRegBit(volatile uint8_t* reg, uint8_t pin, bool state);
void setPin(Pin pin, PinState state);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_PIN_H */

