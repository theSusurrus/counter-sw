/*
 * File:   pin.c
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 12:08
 */

#include "main.h"

#include <util/delay.h>

#include "pin.h"

void setRegBit(volatile uint8_t* reg, uint8_t pin, bool state) {
    if(state) {
        *reg |= 1 << pin;
    } else {
        *reg &= ~(1 << pin);
    }
}

void setGPIO(GPIO pin, GPIOState state) {
    switch(state) {
        case TRISTATE:
            setRegBit(pin.ddr, pin.index, false);
            setRegBit(pin.port, pin.index, false);
            break;
        case OUTPUT_SOURCE:
            setRegBit(pin.ddr, pin.index, true);
            setRegBit(pin.port, pin.index, true);
            break;
        case OUTPUT_SINK:
            setRegBit(pin.ddr, pin.index, true);
            setRegBit(pin.port, pin.index, false);
            break;
        case INPUT_SOURCING:
            setRegBit(pin.ddr, pin.index, false);
            setRegBit(pin.port, pin.index, true);
            break;
    }
}

static bool readGPIORaw(GPIO pin) {
    return (*pin.pin & (1 << pin.index)) == 0;
}

bool readGPIO(GPIO pin) {
    bool pressed = readGPIORaw(pin);
    if(pressed) {
        _delay_ms(5);
        readGPIORaw(pin);
    }
    return pressed;
}

GPIO CCD1 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = &PIND,
    .index = 5,
};

GPIO CCD2 = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = &PINC,
    .index = 0,
};

GPIO CCD3 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = &PIND,
    .index = 0,
};

GPIO CCD4 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = &PIND,
    .index = 4,
};

GPIO CCL12 = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = &PINC,
    .index = 3,
};

GPIO CCL3 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = &PIND,
    .index = 1,
};

GPIO AAL123 = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = &PIND,
    .index = 3,
};

GPIO AB = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = &PIND,
    .index = 6,
};

GPIO AC = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = &PINC,
    .index = 2,
};

GPIO AD = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = &PINC,
    .index = 1,
};

GPIO AE = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = &PINC,
    .index = 4,
};

GPIO AF = {
    .port = &PORTC,
    .ddr = &DDRC,
    .pin = &PINC,
    .index = 5,
};

GPIO AG = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = &PIND,
    .index = 7,
};

GPIO ADP = {
    .port = &PORTD,
    .ddr = &DDRD,
    .pin = &PIND,
    .index = 2,
};

GPIO SW1 = {
    .port = &PORTB,
    .ddr = &DDRB,
    .pin = &PINB,
    .index = 6,
};

GPIO SW2 = {
    .port = &PORTB,
    .ddr = &DDRB,
    .pin = &PINB,
    .index = 7,
};

GPIO SW3 = {
    .port = &PORTB,
    .ddr = &DDRB,
    .pin = &PINB,
    .index = 0,
};

GPIO SW4 = {
    .port = &PORTB,
    .ddr = &DDRB,
    .pin = &PINB,
    .index = 1,
};

GPIO SW5 = {
    .port = &PORTB,
    .ddr = &DDRB,
    .pin = &PINB,
    .index = 2,
};

GPIO* SW[5] = {
    &SW1,
    &SW2,
    &SW3,
    &SW4,
    &SW5,
};
