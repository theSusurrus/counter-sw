/*
 * File:   timekeeping.h
 * Author: Jakub Dudarewicz
 *
 * Created on 1 october 2024, 14:04
 */

#ifndef _TIMEKEEPING_H_
#define _TIMEKEEPING_H_

#include "main.h"

typedef int64_t TickDifference;
typedef uint64_t Tick;

Tick getTick();

#endif /* _TIMEKEEPING_H_ */