/*
 * File:   main.c
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 11:46
 */

#include "main.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

#include "pin.h"
#include "led_driver.h"
#include "modes.h"
#include "timekeeping.h"

void init() {
    SFIOR |= (1 << PUD); // disable pullups
    for(int i = 0; i < SW_NUMBER; i++) {
        setGPIO(*SW[i], TRISTATE); // enable inputs on switches
    }
    

   // init Timer1 with Fast PWM and ICR for compare mode (14) to get long periods
   TIMSK  |= (1 << TICIE1);                                    // set input capture interrupt enable
   TCCR1A |= (1 << WGM11);                                     // set Fast PWM mode with ICR1 as compare register
   TCCR1B |= (1 << WGM13) | (1 << WGM12) | TIMER1_PRESCALER;   // set Fast PWM mode with ICR1 as compare register
   ICR1H   = (TIMER1_COMPARE_VALUE >> 8);                      // set compare value for interrupt
   ICR1L   = (TIMER1_COMPARE_VALUE & 0xFF);                    // set compare value for interrupt

   // init Timer2 as interrupt generator for display refresh
   TIMSK |= (1 << OCIE2);                    // set output compare interrupt enable
   TCCR2 |= (1 << WGM21) | TIMER2_PRESCALER; // set CTC mode
   OCR2   = TIMER2_COMPARE_VALUE;            // set compare value for interrupt

   sei(); // enable interrupts
}

int main(void) {
    init();
    bool last_mode_change = false;
    size_t active_mode = 0;
    Tick tick_at_first_pushed = 0;

    while(1) {
        bool mode_change = readGpioNonblockingReleased(SW5, &last_mode_change, &tick_at_first_pushed);

        if(mode_change) {
            ModeHandler handler = Handlers[active_mode + 1];
            if(handler != NULL) {
                active_mode++;
            } else {
                active_mode = 0;
            }
        }

        Handlers[active_mode]();

        handleLEDInterrupt();

        last_mode_change = mode_change;
    };
    
    return 0;
}
