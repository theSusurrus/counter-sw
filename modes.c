/*
 * File:   modes.c
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 14:04
 */

#include "main.h"

#include <util/delay.h>

#include "modes.h"
#include "led_driver.h"
#include "pin.h"
#include "timekeeping.h"

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
    bool last_pushed = false;
    Tick tick_at_first_pushed = false;

    for(int i = 0; i < SW_NUMBER; i++) {
        sw[i] = readGpioNonblocking(*SW[i], &last_pushed, &tick_at_first_pushed);
        string[i] = sw[i] ? '1' : '0';
    }

    displayLED(string);
}

static inline void counterRefresh(uint16_t counter) {
    char string[5] = "    ";
    integerToString(counter, string);
    displayLED(string);
}

void counterHandler() {
    static uint16_t counter = 0;
    static uint8_t last_switch = 0xFF;
    static uint16_t last_tick = 0;
    static Tick cumulative_tick = 0;
    static uint16_t counter_accumulated_increment = 0;

    bool any_pushed = false;
    bool long_push = false;
    int increment = 0;

    for(int sw_i = 0; sw_i < 4; sw_i++) {
        const uint16_t ones_tick_delay = 5;
        // const uint16_t tens_tick_delay = 10;
        // const uint16_t hundreds_tick_delay = 15;
        const uint16_t tens_unit_delay = 10;
        const uint16_t hundreds_unit_delay = 50;

        if(readGpioBlocking(*SW[sw_i])) {
            /* a button pushed in */
            any_pushed = true;
            Tick tick = getTick();

            if(last_switch != sw_i) {
                /* reset switch hold state if new switch */
                last_tick = tick;
                cumulative_tick = 0;
                counter_accumulated_increment = 0;
            }
            last_switch = sw_i;

            /* count time since last update */
            TickDifference elapsed = tick - last_tick;
            last_tick = tick;

            /* accumulate tick */
            cumulative_tick += elapsed;

            /* increment direction */
            if(sw_i == 2) {
                increment = -1;
            } else if(sw_i == 3) {
                increment = 1;
            } else {
                continue;
            }

            do {
                if(cumulative_tick < ones_tick_delay){
                    /* wait for short time before we start incrementing while holding */
                    break;
                }
                if(cumulative_tick >= ones_tick_delay) {
                    long_push = true;
                    /* single push wait elapsed, start incrementing by at least 1 */
                    uint16_t scaled_increment = increment;
                    if(counter_accumulated_increment > tens_unit_delay) {
                        /* long press, start incrementing by 10  */
                        scaled_increment = 10 * increment;
                        counterRefresh(counter);
                    } else if (counter_accumulated_increment > hundreds_unit_delay) {
                        /* longer press, start incrementing by 100*/
                        scaled_increment = 100 * increment;
                        counterRefresh(counter);
                    }
                    counter += scaled_increment;
                    counter_accumulated_increment += scaled_increment;
                }
            } while(0);
        }
    }

    if(any_pushed == false) {
        /* button not pushed in */
        if ((last_switch == 2 || last_switch == 3) && long_push == false) {
            /* single push, increment */
            counter += increment;
        }
        last_switch = 0xFF;
    }

    counterRefresh(counter);
}
void tickHandler() {
    Tick tick = getTick();
    char string[5] = "    ";
    integerToString(tick, string);
    displayLED(string);
}

ModeHandler Handlers[] = {
    tickHandler,
    counterHandler,
    incrementHandler,
    switchDemoHandler,
    0,
};

