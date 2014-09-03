#include "stdafx.h"
#include "DeFuzzy.h"

float DeFuzzy::centroidMax(FuzzyFunction* varOut, float *output) {
    float max = 0.0, max_ = 0.0;
    float denominador = 0;
    float numerador = 0;
    const float MAX = varOut->getVmax();
    const float dx = varOut->getDx();
    for (float j = varOut->getVmin(); j <= MAX; j += dx) {
        for (int i = 0; i < varOut->getnParticoes(); i++) {
            max_ = output[i] * varOut->singleton(i, j);
            if (max_ > max)
                max = max_;
        }
        denominador += max;
        numerador += max * j;
        max = 0;
    }
    free(output);
    return fabs(numerador / denominador) > 0.01 ? (numerador / denominador) : 0;
}

float DeFuzzy::centroidMax2(FuzzyFunction* varOut, float *output) {
    float max = 0.0, max_ = 0.0;
    float denominador = 0;
    float numerador = 0;
    //    const float min = varOut->getVmin();
    const float MAX = varOut->getVmax();
    const float dx = varOut->getDx();
    for (float j = varOut->getVmin(); j <= MAX; j += dx) {
        for (int i = 0; i < varOut->getnParticoes(); i++) {
            max_ = varOut->singleton(i, j);
            if (output[i] < max_)
                max_ = output[i];
            if (max_ > max)
                max = max_;
        }
        denominador += max;
        numerador += max * j;
        max = 0.0;
    }
    free(output);
    return fabs(numerador / denominador) > 0.01 ? (numerador / denominador) : 0;
}

float DeFuzzy::centroidSum(FuzzyFunction *varOut, float *output) {
    float numerador = 0;
    float denominador = 0;
    float aux = 0;
    const float min = varOut->getVmin();
    const float max = varOut->getVmax();
    const float dx = varOut->getDx();
    for (float j = min; j <= max; j += dx) {
        for (int i = 0; i < varOut->getnParticoes(); i++) {
            aux = output[i] * varOut->singleton(i, j);
            numerador += j * aux;
            denominador += aux;
        }
    }
    free(output);
    return fabs(numerador / denominador) > 0.01 ? numerador / denominador : 0;
}

