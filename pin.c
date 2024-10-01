/*
 * File:   pin.c
 * Author: jakub
 *
 * Created on 1 pa?dziernika 2024, 12:08
 */

#include "pin.h"

void setRegBit(volatile uint8_t* reg, uint8_t pin, bool state) {
    if(state) {
        *reg |= 1 << pin;
    } else {
        *reg &= ~(1 << pin);
    }
}

void setPin(Pin pin, PinState state) {
    switch(state) {
        case TRISTATE:
            setRegBit(pin.ddr, pin.pin, false);
            setRegBit(pin.port, pin.pin, false);
            break;
        case OUTPUT_SOURCE:
            setRegBit(pin.ddr, pin.pin, true);
            setRegBit(pin.port, pin.pin, true);
            break;
        case OUTPUT_SINK:
            setRegBit(pin.ddr, pin.pin, true);
            setRegBit(pin.port, pin.pin, false);
            break;
        case INPUT_SOURCING:
            setRegBit(pin.ddr, pin.pin, false);
            setRegBit(pin.port, pin.pin, true);
            break;
    }
}

Pin CCD1 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = 5,
};

Pin CCD2 = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = 0,
};

Pin CCD3 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = 0,
};

Pin CCD4 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = 4,
};

Pin CCL12 = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = 3,
};

Pin CCL3 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = 1,
};

Pin AAL123 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = 3,
};

Pin AB = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = 6,
};

Pin AC = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = 2,
};

Pin AD = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = 1,
};

Pin AE = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = 4,
};

Pin AF = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = 5,
};

Pin AG = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = 7,
};

Pin ADP = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = 2,
};
