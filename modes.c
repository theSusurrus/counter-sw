/*
 * File:   modes.c
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 14:04
 */

#include "main.h"
#include "modes.h"
#include "pin.h"

static void integerToString(uint16_t i, char s[5]) {
    s[0] = (i / 1000) % 10 + '0';
    s[1] = (i / 100) % 10 + '0';
    s[2] = (i / 10)% 10 + '0';
    s[3] = (i % 10) + '0';
}

void demoHandler() {
    bool sw[SW_NUMBER];
    char string[5] = "    ";

    for(int i = 0; i < SW_NUMBER; i++) {
        sw[i] = readGPIO(*SW[i]);
        string[i] = sw[i] ? '1' : '0';
    }

    displayLED(string);
}

uint8_t counter = 0;

void counterHandler() {
    bool sw[SW_NUMBER];
    char string[5] = "    ";

    for(int i = 0; i < SW_NUMBER; i++) {
        sw[i] = readGPIO(*SW[i]);
    }

    if(sw[3]) {
        counter++;
    } else if(sw[2]) {
        counter--;
    }

    integerToString(counter, string);

    displayLED(string);
}

ModeHandler Handlers[] = {
    demoHandler,
    counterHandler,
    0,
};

