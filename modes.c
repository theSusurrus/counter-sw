/*
 * File:   modes.c
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 14:04
 */

#include "main.h"

#include <avr/delay.h>

#include "modes.h"
#include "led_driver.h"
#include "pin.h"

static void integerToString(uint16_t i, char s[5]) {
    s[0] = (i / 1000) % 10 + '0';
    s[1] = (i / 100) % 10 + '0';
    s[2] = (i / 10)% 10 + '0';
    s[3] = (i % 10) + '0';
}

void incrementHandler() {
    static uint16_t counter = 0;
    static char string[5] = "    ";

    if(counter > 9999) {
        counter = 0;
    }

    counter++;

    integerToString(counter, string);
    displayLED(string);
}

void switchDemoHandler() {
    bool sw[SW_NUMBER];
    char string[5] = "    ";

    for(int i = 0; i < SW_NUMBER; i++) {
        sw[i] = readGPIO(*SW[i]);
        string[i] = sw[i] ? '1' : '0';
    }

    displayLED(string);
}

void counterHandler() {
    static uint16_t counter = 0;
    char string[5] = "    ";

    for(int i = 0; i < 4; i++) {
        bool tmp = readGPIO(*SW[i]);
        uint32_t short_delay = 0;
        int increment = 0;
        const uint16_t short_delay_ms = 10;
        const uint16_t long_delay_ms = 100;
        const uint16_t ones_delay = 20;
        const uint16_t tens_delay = 10;
        const uint16_t hundreds_delay = 15;

        if(i == 2) {
            increment = -1;
        }
        if(i == 3) {
            increment = 1;
        }

        if(tmp) {
            while(short_delay < ones_delay && readGPIO(*SW[i])){
                _delay_ms(short_delay_ms);
                short_delay++;
            }
            if(short_delay >= ones_delay) {
                uint16_t long_delay = 0;
                while(long_delay < tens_delay && readGPIO(*SW[i])) {
                    long_delay++;
                    counter += increment;
                    integerToString(counter, string);
                    displayLED(string);
                    _delay_ms(long_delay_ms);
                }
                while(long_delay < hundreds_delay && readGPIO(*SW[i])) {
                    long_delay++;
                    counter += 10 * increment;
                    integerToString(counter, string);
                    displayLED(string);
                    _delay_ms(long_delay_ms);
                }
                while(readGPIO(*SW[i])) {
                    long_delay++;
                    counter += 100 * increment;
                    integerToString(counter, string);
                    displayLED(string);
                    _delay_ms(long_delay_ms);
                }
            } else {
                counter += increment;
            }
        }
    }

    integerToString(counter, string);
    displayLED(string);
}

ModeHandler Handlers[] = {
    counterHandler,
    incrementHandler,
    switchDemoHandler,
    0,
};

