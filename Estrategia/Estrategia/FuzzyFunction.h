#ifndef FuzzyFunction_H
#define FuzzyFunction_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "stdafx.h"
class FuzzyFunction {
public:
    FuzzyFunction(int nParticoes, int points);
    virtual ~FuzzyFunction();
    virtual float singleton(int indice, float x) = 0;
	virtual const float* const points(const float a, const float b, const float c, const float d)=0;
    void addLabel(int indice, const char * const label, const float * const pontos);
    void setValues(float npoints = 101);
    int getnParticoes() const;
    float getDx() const;
    float getVmax() const;
    float getVmin() const;
	
protected:
    float vmin;
    float vmax;
    float dx;
    float **pontos;
private:
    const char **particoes;
    const int nParticoes;
    const int nPoints;
};

class Gauss : public FuzzyFunction {
public:
    Gauss(int nparticoes);
    ~Gauss();
    void addLabel(int indice, const char * const label, const float * pontos);
    float singleton(int indice, float x);
    const float* const points(const float u, const float sigma);
};

class Trapez : public FuzzyFunction {
public:
    Trapez(int nparticoes);
	Trapez();
    ~Trapez();
    float singleton(int indice, float x);
    const float* const points(const float a, const float b, const float c, const float d);
private:
    float _max(float a, float b);
    float _min(float a, float b);
};

class Bell : public FuzzyFunction {
public:
    Bell(int nparticoes);
    ~Bell();
    float singleton(int indice, float x);
    const float* const points(const float a, const float b, const float c);
};
#endif // FuzzyFunction_H
