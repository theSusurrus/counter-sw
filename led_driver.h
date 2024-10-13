/*
 * File:   led_driver.h
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 14:04
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_LED_DRIVER_H
#define	XC_LED_DRIVER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

void handleLEDInterrupt();
void clearDisplay();
void displayDigits(char string[5]);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_LED_DRIVER_H */

