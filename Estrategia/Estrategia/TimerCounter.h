/* 
 * File:   TimerCounter.h
 * Author: rafael.vaz
 *
 * Created on 8 de Novembro de 2013, 15:14
 */

#ifndef TIMERCOUNTER_H
#define	TIMERCOUNTER_H
#include <windows.h>
#include <stdio.h>

class TimerCounter {
public:
    TimerCounter();
    TimerCounter(const TimerCounter& orig);
    virtual ~TimerCounter();
    double GetCounter();
    void StartCounter();
    double PCFreq;
    __int64 CounterStart;
private:

};

#endif	/* TIMERCOUNTER_H */

