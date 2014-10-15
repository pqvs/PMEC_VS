/* 
 * File:   DeFuzzy.h
 * Author: rafael.vaz
 *
 * Created on 28 de Fevereiro de 2014, 20:55
 */

#ifndef DEFUZZY_H
#define	DEFUZZY_H
#include "FuzzyFunction.h"

class DeFuzzy {
public:
    static float centroidMax(FuzzyFunction* varOut, float *output);
    static float centroidMax2(FuzzyFunction* varOut, float *output);
    static float centroidSum(FuzzyFunction* varOut, float *output);

};

#endif	/* DEFUZZY_H */

