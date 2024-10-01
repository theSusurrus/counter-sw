/*
 * File:   main.c
 * Author: jakub
 *
 * Created on 1 pa?dziernika 2024, 11:46
 */

#define F_CPU 1000000UL

#include <xc.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "pin.h"
#include "led_driver.h"

void integerToString(uint16_t i, char s[5]) {
    s[0] = (i / 1000) % 10 + '0';
    s[1] = (i / 100) % 10 + '0';
    s[2] = (i / 10)% 10 + '0';
    s[3] = (i % 10) + '0';
}

int main(void) {
    SFIOR |= (1 << PUD);
    char string[5] = "1234";
    int i = 0;
    int j = 0;
    while(1) {
        displayLED(string);
        i += 1;
        if (i == 5) {
            i = 0;
            j += 1;
            integerToString(j);
        }
    };
    
    return 0;
}
