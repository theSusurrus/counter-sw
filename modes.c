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

static void integer4DigitToString(uint16_t i, char s[5]) {
    s[0] = i < 1000 ? ' ' : (i / 1000) % 10 + '0';
    s[1] = i < 100 ? ' ' : (i / 100) % 10 + '0';
    s[2] = i < 10 ? ' ' : (i / 10)% 10 + '0';
    s[3] = (i % 10) + '0';
}

static void integer2DigitToString(uint16_t i, char s[2]) {
    s[0] = i < 10 ? ' ' : (i / 10)% 10 + '0';
    s[1] = (i % 10) + '0';
}

void switchDemoHandler() {
    char string[5] = "    ";
    static bool last_pushed[4] = {0};
    static Tick tick_at_first_pushed[4] = {0};

    for(int i = 0; i < 4; i++) {
        bool pushed = readGpioNonblocking(*SW[i], &last_pushed[i], &tick_at_first_pushed[i]);
        // bool pushed = readGpioRaw(*SW[i]);
        string[i] = pushed ? '1' : '0';
    }

    displayDigits(string);
}

static inline void refreshCounter(uint16_t counter) {
    char string[5] = "    ";
    integer4DigitToString(counter, string);
    displayDigits(string);
}

static inline uint16_t addToCounter(uint16_t counter, uint16_t increment) {
    uint16_t result = counter + increment;
    if(result > 9999) {
        result = 0;
    }
    return result;
}

void counterHandler() {
    static uint16_t counter = 0;
    static uint8_t last_switch = 0xFF;
    static uint16_t last_tick = 0;
    static Tick cumulative_tick = 0;
    static uint16_t increment_events = 0;
    static Tick tick_at_last_increment = 0;
    static bool first_incremented = false;

    static const Tick initial_tick_delay = 30;
    static const Tick tick_delay = 50;

    bool any_pushed = false;
    bool long_push = false;
    int increment = 0;

    static bool last_pushed[4] = {0};
    static Tick tick_at_first_pushed[4] = {0};

    for(int sw_i = 0; sw_i < 4; sw_i++) {
        const uint16_t tens_increments_delay = 9;
        const uint16_t hundreds_increments_delay = 19;

        if(readGpioNonblocking(*SW[sw_i], &last_pushed[sw_i], &tick_at_first_pushed[sw_i])) {
            /* a button pushed in */
            any_pushed = true;
            Tick now = getTick();

            if(last_switch != sw_i) {
                /* reset switch hold state if new switch */
                last_tick = now;
                cumulative_tick = 0;
                increment_events = 0;
                tick_at_last_increment = 0;
                first_incremented = false;
            }
            last_switch = sw_i;

            /* count time since last update */
            TickDifference elapsed = now - last_tick;
            last_tick = now;

            /* accumulate tick */
            cumulative_tick += elapsed;

            do {
                /* increment direction */
                if(sw_i == 2) {
                    increment = -1;
                } else if(sw_i == 3) {
                    increment = 1;
                } else if (sw_i == 0) {
                    counter = 0;
                    refreshCounter(counter);
                    continue;
                } else {
                    continue;
                }

                if(cumulative_tick < initial_tick_delay){
                    if(!first_incremented) {
                        counter = addToCounter(counter, increment);
                        first_incremented = true;
                    }
                    /* wait for short time before we start incrementing while holding */
                    continue;
                }

                if(cumulative_tick >= tick_delay + initial_tick_delay) {
                    long_push = true;
                    /* single push wait elapsed, start incrementing by at least 1 */
                    uint16_t scaled_increment = increment;
                    if(increment_events > tens_increments_delay &&
                       increment_events <= hundreds_increments_delay) {
                        /* long press, start incrementing by 10  */
                        scaled_increment = 10 * increment;
                    } else if (increment_events > hundreds_increments_delay) {
                        /* longer press, start incrementing by 100*/
                        scaled_increment = 100 * increment;
                    }
                    
                    TickDifference diff = now - tick_at_last_increment;
                    if(diff >= tick_delay) {
                        tick_at_last_increment = now;
                        increment_events++;
                        counter = addToCounter(counter, scaled_increment);
                    }
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
    refreshCounter(counter);
}

static inline void refreshTapper(uint8_t tapped, uint8_t untapped) {
    char string[5] = "    ";
}

typedef enum TapperButton_T {
    UNTAP = 0,
    TAP = 1,
    REMOVE = 2,
    ADD = 3,
    NONE
} TapperButton;

static void handleTapperButton(TapperButton button,
                          uint8_t* tapped,
                          uint8_t* untapped,
                          uint8_t increment) {
    switch(button) {
    case TAP:
        if(*untapped > 0) {
            (*tapped)++;
            (*untapped)--;
        }
        break;
    case UNTAP:
        if(*tapped > 0) {
            (*tapped)--;
            (*untapped)++;
        }
        break;
    case REMOVE:
        if(*tapped > 0) {
            (*tapped)--;
        } else if(untapped > 0) {
            (*untapped)--;
        }
        break;
    case ADD:
        if(*tapped + *untapped < 99) {
            (*untapped)++;
        }
        break;
    default:
        break;
    }
}

void tapperHandler() {
    static uint8_t untapped = 0;
    static uint8_t tapped = 0;
    static uint8_t last_button = 0xFF;
    static uint16_t last_tick = 0;
    static Tick cumulative_tick = 0;
    static uint16_t increment_events = 0;
    static Tick tick_at_last_increment = 0;
    static bool first_incremented = false;

    static const Tick initial_tick_delay = 30;
    static const Tick tick_delay = 50;

    TapperButton pushed = false;
    bool long_push = false;

    static bool last_pushed[4] = {0};
    static Tick tick_at_first_pushed[4] = {0};

    for(TapperButton button = 0; button < 4; button++) {
        const uint16_t tens_increments_delay = 9;

        if(readGpioNonblocking(*SW[button], &last_pushed[button], &tick_at_first_pushed[button])) {
            /* a button pushed in */
            pushed = button;
            Tick now = getTick();

            if(last_button != button) {
                /* reset switch hold state if new switch */
                last_tick = now;
                cumulative_tick = 0;
                increment_events = 0;
                tick_at_last_increment = 0;
                first_incremented = false;
            }
            last_button = button;

            /* count time since last update */
            TickDifference elapsed = now - last_tick;
            last_tick = now;

            /* accumulate tick */
            cumulative_tick += elapsed;

            do {
                if(cumulative_tick < initial_tick_delay){
                    if(!first_incremented) {
                        handleTapperButton(button, &tapped, &untapped, 1);
                        first_incremented = true;
                    }
                    /* wait for short time before we start incrementing while holding */
                    continue;
                }

                if(cumulative_tick >= tick_delay + initial_tick_delay) {
                    long_push = true;
                    /* single push wait elapsed, start incrementing by at least 1 */
                    uint16_t scaled_increment = 1;
                    if(increment_events > tens_increments_delay) {
                        /* long press, start incrementing by 10  */
                        scaled_increment = 10;
                    }
                    
                    TickDifference diff = now - tick_at_last_increment;
                    if(diff >= tick_delay) {
                        tick_at_last_increment = now;
                        increment_events++;
                        handleTapperButton(button, &tapped, &untapped, scaled_increment);
                    }
                }
            } while(0);
        }
    }

    if(pushed == NONE) {
        /* button not pushed in */
        if (last_button != NONE && long_push == false) {
            /* single push */
            handleTapperButton(last_button, &tapped, &untapped, 1);
        }
        last_button = NONE;
    }
    refreshTapper(tapped, untapped);
}

void tickHandler() {
    Tick tick = getTick();
    char string[5] = "    ";
    integer4DigitToString(tick, string);
    displayDigits(string);
}

void stopwatchHandler() {
    Tick tick = getTick();
    char seconds[5] = "    ";
    integer4DigitToString((((uint16_t)tick) / (uint16_t)250) % (uint16_t)60, seconds);
    displayDigits(seconds);
}

ModeHandler Handlers[] = {
    tapperHandler,
    counterHandler,
    switchDemoHandler,
    tickHandler,
    stopwatchHandler,
    0,
};

