/*
 * File:   pin.c
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 12:08
 */

#include "main.h"

#include <util/delay.h>

#include "pin.h"
#include "led_driver.h"

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

bool readGpioRaw(GPIO pin) {
    return (*pin.pin & (1 << pin.index)) == 0;
}

bool readGpioBlocking(GPIO pin) {
    bool pressed = readGpioRaw(pin);
    if(pressed) {
        _delay_ms(5);
        readGpioRaw(pin);
    }
    return pressed;
}

/**
 * Returns true after a button has been held for a moment
 */
bool readGpioNonblocking(GPIO pin, bool* last_pushed, Tick* tick_at_first_pushed) {
    bool debounce_finished = false;
    bool pushed = readGpioRaw(pin);

    if(pushed) {
        do {
            if(last_pushed == false) {
                *tick_at_first_pushed = getTick();
            } else {
                TickDifference diff = getTick() - *tick_at_first_pushed;
                if(diff < 2) {
                    /* at least two ticks before changing modes */
                    debounce_finished = true;
                    break;
                }
            }
        } while(0);
    }

    return debounce_finished;
}

/**
 * Returns true after a button has been held for a moment and released
 */
bool readGpioNonblockingReleased(GPIO pin, bool* last_pushed, Tick* tick_at_first_pushed) {
    bool debounce_finished = false;
    bool pushed = readGpioRaw(pin);

    if(pushed) {
        do {
            if(*last_pushed == false) {
                *tick_at_first_pushed = getTick();
            } else {
                TickDifference diff = getTick() - *tick_at_first_pushed;
                if(diff < 2) {
                    /* at least two ticks before changing modes */
                    debounce_finished = true;
                    break;
                }
            }
        } while(0);
    } else {
        if(*last_pushed == true && debounce_finished == true) {
            return true;
        }
    }

    *last_pushed = pushed;

    return false;
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
