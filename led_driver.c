/*
 * File:   led_driver.c
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 14:04
 */

#include "main.h"

#include <util/delay.h>
#include <stdint.h>

#include "led_driver.h"
#include "pin.h"

typedef struct {
    uint8_t AL123 : 1;
    uint8_t B : 1;
    uint8_t C : 1;
    uint8_t D : 1;
    uint8_t E : 1;
    uint8_t F : 1;
    uint8_t G : 1;
    uint8_t DP : 1;
} Segments;

static void SetAnode(GPIO pin, uint8_t state) {
    setGPIO(pin, state == 1 ? OUTPUT_SOURCE : OUTPUT_SINK);
}

static void setAnodes(Segments segments) {
    SetAnode(AAL123, segments.AL123);
    SetAnode(AB, segments.B);
    SetAnode(AC, segments.C);
    SetAnode(AD, segments.D);
    SetAnode(AE, segments.E);
    SetAnode(AF, segments.F);
    SetAnode(AG, segments.G);
    SetAnode(ADP, segments.DP);
}

Segments CharA = {
    .AL123 = 0,
    .B = 1,
    .C = 1,
    .D = 1,
    .E = 1,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments Charb = {
    .AL123 = 1,
    .B = 1,
    .C = 1,
    .D = 0,
    .E = 0,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments CharC = {
    .AL123 = 1,
    .B = 1,
    .C = 1,
    .D = 1,
    .E = 0,
    .F = 0,
    .G = 0,
    .DP = 0,
};

Segments CharE = {
    .AL123 = 1,
    .B = 1,
    .C = 1,
    .D = 1,
    .E = 0,
    .F = 0,
    .G = 1,
    .DP = 0,
};

Segments CharF = {
    .AL123 = 0,
    .B = 1,
    .C = 1,
    .D = 1,
    .E = 0,
    .F = 0,
    .G = 1,
    .DP = 0,
};

Segments CharH = {
    .AL123 = 0,
    .B = 1,
    .C = 1,
    .D = 0,
    .E = 1,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments CharL = {
    .AL123 = 0,
    .B = 1,
    .C = 1,
    .D = 0,
    .E = 1,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments Charn = {
    .AL123 = 0,
    .B = 1,
    .C = 0,
    .D = 0,
    .E = 0,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments Char0 = {
    .AL123 = 1,
    .B = 1,
    .C = 1,
    .D = 1,
    .E = 1,
    .F = 1,
    .G = 0,
    .DP = 0,
};

Segments Char1 = {
    .AL123 = 0,
    .B = 0,
    .C = 0,
    .D = 0,
    .E = 1,
    .F = 1,
    .G = 0,
    .DP = 0,
};

Segments Char2 = {
    .AL123 = 1,
    .B = 1,
    .C = 0,
    .D = 1,
    .E = 1,
    .F = 0,
    .G = 1,
    .DP = 0,
};

Segments Char3 = {
    .AL123 = 1,
    .B = 0,
    .C = 0,
    .D = 1,
    .E = 1,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments Char4 = {
    .AL123 = 0,
    .B = 0,
    .C = 1,
    .D = 0,
    .E = 1,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments Char5 = {
    .AL123 = 1,
    .B = 0,
    .C = 1,
    .D = 1,
    .E = 0,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments Char6 = {
    .AL123 = 1,
    .B = 1,
    .C = 1,
    .D = 1,
    .E = 0,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments Char7 = {
    .AL123 = 0,
    .B = 0,
    .C = 0,
    .D = 1,
    .E = 1,
    .F = 1,
    .G = 0,
    .DP = 0,
};

Segments Char8 = {
    .AL123 = 1,
    .B = 1,
    .C = 1,
    .D = 1,
    .E = 1,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments Char9 = {
    .AL123 = 1,
    .B = 0,
    .C = 1,
    .D = 1,
    .E = 1,
    .F = 1,
    .G = 1,
    .DP = 0,
};

Segments CharSpace = {
    .AL123 = 0,
    .B = 0,
    .C = 0,
    .D = 0,
    .E = 0,
    .F = 0,
    .G = 0,
    .DP = 0,
};

static void clear() {
    setAnodes(CharSpace);
    setGPIO(CCD1, TRISTATE);
    setGPIO(CCD2, TRISTATE);
    setGPIO(CCD3, TRISTATE);
    setGPIO(CCD4, TRISTATE);
    setGPIO(CCL12, TRISTATE);
    setGPIO(CCL3, TRISTATE);
}

static void setCharacter(char c) {
    switch (c) {
        case 'A':
            setAnodes(CharA);
            break;
        case 'b':
            setAnodes(Charb);
            break;
        case 'C':
            setAnodes(CharC);
            break;
        case 'F':
            setAnodes(CharF);
            break;
        case 'H':
            setAnodes(CharH);
            break;
        case 'L':
            setAnodes(CharL);
            break;
        case '0':
            setAnodes(Char0);
            break;
        case '1':
            setAnodes(Char1);
            break;
        case '2':
            setAnodes(Char2);
            break;
        case '3':
            setAnodes(Char3);
            break;
        case '4':
            setAnodes(Char4);
            break;
        case '5':
            setAnodes(Char5);
            break;
        case '6':
            setAnodes(Char6);
            break;
        case '7':
            setAnodes(Char7);
            break;
        case '8':
            setAnodes(Char8);
            break;
        case '9':
            setAnodes(Char9);
            break;
        default:
            clear();
            break;
    }
}

#define REFRESH_DELAY_US 1000

void displayLED(char string[5]) {
    clear();
    setCharacter(string[0]);
    setGPIO(CCD4, OUTPUT_SINK);
    _delay_us(REFRESH_DELAY_US);
    
    clear();
    setCharacter(string[1]);
    setGPIO(CCD3, OUTPUT_SINK);
    _delay_us(REFRESH_DELAY_US);
    
    clear();
    setCharacter(string[2]);
    setGPIO(CCD2, OUTPUT_SINK);
    _delay_us(REFRESH_DELAY_US);
    
    clear();
    setCharacter(string[3]);
    setGPIO(CCD1, OUTPUT_SINK);
    _delay_us(REFRESH_DELAY_US);
    
//    clear();
}
