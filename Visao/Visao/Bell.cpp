#include "FuzzyFunction.h"

Bell::Bell(int nParticoes) : FuzzyFunction(nParticoes, 3) {
    
}

Bell::~Bell() {
    //dtor
//    for (int i = 0; i < 3; i++)
//        free(pontos[i]);
//    free(pontos);
}

//void Bell::addLabel(int indice, const char * const label, const float * pontos) {
//    // Armazena os valores da vari�vel em seus respectivos arrays.
//    this->particoes[indice] = label;
//
//    this->pontos[indice][0] = pontos[0];
//    this->pontos[indice][1] = pontos[1];
//    
//    free((float*) pontos);
//}

float Bell::singleton(int indice, float x) {
    return 1 / (1 + abs(pow((x - this->pontos[indice][2]) / this->pontos[indice][0], 2 * this->pontos[indice][1])));
}

const float* const Bell::points(const float a, const float b, const float c) {
    float* array = new float[3];
    array[0] = a;
    array[1] = b;
    array[2] = c;
    return (const float *) array;
}

//void Bell::setValues(float vmin, float vmax, float npoints) {
//    this->vmin = vmin;
//    this->vmax = vmax;
//    //this->dx = dx;
//    this->dx = (vmax - vmin) / npoints;
//}






