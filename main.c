/*
 * File:   main.c
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 11:46
 */

#include <xc.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "main.h"
#include "pin.h"
#include "led_driver.h"
#include "modes.h"

void init() {
    SFIOR |= (1 << PUD);
    for(int i = 0; i < SW_NUMBER; i++) {
        setGPIO(*SW[i], TRISTATE);
    }
}

int main(void) {
    init();

    size_t active_mode = 0;

    while(1) {
        bool mode_change = readGPIO(SW5);

        if(mode_change) {
            ModeHandler handler = Handlers[active_mode + 1];
            if(handler != NULL) {
                active_mode++;
            } else {
                active_mode = 0;
            }
        }

        Handlers[active_mode]();
    };
    
    return 0;
}
