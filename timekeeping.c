/*
 * File:   timekeeping.h
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 14:04
 */

#include "timekeeping.h"

#include <avr/interrupt.h>

static Tick tick = 0;

Tick getTick() {
    return tick;
}

ISR(TIMER1_CAPT_vect) {
    tick++;
}

