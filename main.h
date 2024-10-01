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

#define TIMER1_PRESCALER      (1 << CS11)
#define TIMER1_COMPARE_VALUE  0x0200

#define F_CPU 1000000UL

#endif
