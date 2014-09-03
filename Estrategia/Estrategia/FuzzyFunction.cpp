#include "stdafx.h"
#include "FuzzyFunction.h"
#include <iostream>

FuzzyFunction::FuzzyFunction(int nParticoes, int points) : nParticoes(nParticoes), nPoints(points) {
    // Aloca��o para o array de strings
    this->particoes = (const char**) calloc(nParticoes, sizeof (const char*));
    
    // Aloca��o para o array de pontos
    this->pontos = (float**) calloc(nParticoes, sizeof (float*));
    for (int i = 0; i < nParticoes; i++)
        this->pontos[i] = (float*) calloc(points, sizeof (float));
}

FuzzyFunction::~FuzzyFunction() {
    //dtor
    for (int i = 0; i < this->nParticoes; i++)
        free((char*) particoes[i]);
    free(particoes);

    for (int i = 0; i < this->nParticoes; i++)
        free((float*)pontos[i]);
    free(pontos);
}

void FuzzyFunction::addLabel(int indice, const char * const label, const float * const pontos) {
    this->particoes[indice] = (const char*) calloc(strlen(label)+1, sizeof (const char));
    strcpy((char*) this->particoes[indice], label);
    
    for (int i = 0; i < this->nPoints; i++) {
        this->pontos[indice][i] = pontos[i];
    }
    //    this->pontos[indice][0] = pontos[0];
    //    this->pontos[indice][1] = pontos[1];
    //    this->pontos[indice][2] = pontos[2];
    //    this->pontos[indice][3] = pontos[3];

    //free((float*) pontos);
}

void FuzzyFunction::setValues(float npoints) {
    this->vmin = this->pontos[0][0];
    this->vmax = this->vmin;
    for (int i = 0; i<this->nParticoes; i++) {
        for (int j = 0; j < this->nPoints; j++) {
            if (this->pontos[i][j]<this->vmin) {
                this->vmin = this->pontos[i][j];
            }
            if (this->pontos[i][j]>this->vmax) {
                this->vmax = this->pontos[i][j];
            }
        }
    }
    //    this->vmin = vmin;
    //    this->vmax = vmax;
    //this->dx = dx;
    this->dx = (vmax - vmin) / (npoints-1);
}

int FuzzyFunction::getnParticoes() const {
    return nParticoes;
}

float FuzzyFunction::getDx() const {
    return dx;
}

float FuzzyFunction::getVmax() const {
    return vmax;
}

float FuzzyFunction::getVmin() const {
    return vmin;
}
