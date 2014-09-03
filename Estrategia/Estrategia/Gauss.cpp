#include "stdafx.h"
#include "FuzzyFunction.h"

Gauss::Gauss(int nParticoes) : FuzzyFunction(nParticoes, 2) {

}

Gauss::~Gauss() {
    //dtor
//    for (int i = 0; i < 2; i++)
//        free(pontos[i]);
//    free(pontos);
}

//void Gauss::addLabel(int indice, const char * const label, const float * pontos) {
//    // Armazena os valores da vari�vel em seus respectivos arrays.
//    this->particoes[indice] = label;
//
//    this->pontos[indice][0] = pontos[0];
//    this->pontos[indice][1] = pontos[1];
//    
//    free((float*) pontos);
//}

float Gauss::singleton(int indice, float x) {
    return (float)exp(-0.5 * pow((x - this->pontos[indice][0]) / this->pontos[indice][1], 2));
}

const float* const Gauss::points(const float u, const float sigma) {
    float* array = new float[2];
    array[0] = u;
    array[1] = sigma;
    return (const float *)array;
}

//void Gauss::setValues(float vmin, float vmax, float npoints) {
//    this->vmin = vmin;
//    this->vmax = vmax;
//    //this->dx = dx;
//    this->dx = (vmax - vmin) / npoints;
//}





