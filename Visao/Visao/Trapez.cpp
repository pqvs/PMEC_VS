#include "FuzzyFunction.h"

Trapez::Trapez(int nParticoes) : FuzzyFunction(nParticoes, 4) {

}

float Trapez::_max(float a, float b) {
    return (a > b) ? a : b;
}

float Trapez::_min(float a, float b) {
    return (a < b) ? a : b;
}

//void Trapez::setValues(float vmin, float vmax, float npoints) {
//    this->vmin = vmin;
//    this->vmax = vmax;
//    //this->dx = dx;
//    this->dx = (vmax - vmin) / npoints;
//}

//void Trapez::setValues(float npoints) {
//    int size_pontos = sizeof (pontos) / sizeof (pontos[0]);
//    this->vmin = this->pontos[0][0];
//    this->vmax = this->vmin;
//    for (int i = 0; i<this->nParticoes; i++) {
//        for (int j = 0; j < size_pontos; j++) {
//            if (this->pontos[i][j]<this->vmin) {
//                this->vmin = this->pontos[i][j];
//            }
//            if (this->pontos[i][j]>this->vmax) {
//                this->vmax = this->pontos[i][j];
//            }
//        }
//    }
//    //    this->vmin = vmin;
//    //    this->vmax = vmax;
//    //this->dx = dx;
//    this->dx = (vmax - vmin) / npoints;
//}

Trapez::~Trapez() {
    //dtor
}

const float* const Trapez::points(const float a, const float b, const float c, const float d) {
    float* array = new float[4];
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = d;
    return (const float * const) array;
}

float Trapez::singleton(int indice, float x) {

   // return _max(_min(_min((x - this->pontos[indice][0]) / (this->pontos[indice][1] - this->pontos[indice][0]), 1), (this->pontos[indice][3] - x) / (this->pontos[indice][3] - this->pontos[indice][2])), 0);
        float a = this->pontos[indice][0];
        float b = this->pontos[indice][1];
        float c = this->pontos[indice][2];
        float d = this->pontos[indice][3];
        // C�lculo da equa��o do trap�zio
        if (x < a || x >= d)
            return 0;
        if (x>=a && x < b) 
            return (x - a) / (b - a);
          if (x >= b && x < c) 
            return 1;
         if (x>=c && x < d) 
            return (d - x) / (d - c);
        
        return NULL;
}






