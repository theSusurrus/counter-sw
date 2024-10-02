/*
 * File:   main.h
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 14:04
 */

#ifndef _H_MAIN_H_
#define	_H_MAIN_H_

#include <xc.h>
#include <avr/io.h>

/* TIMER1 / 1024 */
#define TIMER1_PRESCALER      (1 << CS11) | (1 << CS10)
/* 1MHz / 64 / 62  ~= 250Hz
   4ms period */
#define TIMER1_COMPARE_VALUE  62

#define TIMER2_PRESCALER      (0 << CS22) | (1 << CS21) | (0 << CS20)
#define TIMER2_COMPARE_VALUE  80

#define F_CPU 1000000UL

#endif
