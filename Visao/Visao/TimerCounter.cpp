/* 
 * File:   TimerCounter.cpp
 * Author: rafael.vaz
 * 
 * Created on 8 de Novembro de 2013, 15:14
 */

#include "stdafx.h"
#include "TimerCounter.h"
TimerCounter::TimerCounter() {
    this->CounterStart = (__int64)0.0;
    this->PCFreq = 0.0;
}

TimerCounter::TimerCounter(const TimerCounter& orig) {
}

TimerCounter::~TimerCounter() {
}

void TimerCounter::StartCounter() {
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        printf("QueryPerformanceFrequency failed!\n");

    PCFreq = double(li.QuadPart) / 1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double TimerCounter::GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

