#pragma once
#include "Point.h"
class Objeto
{
public:
	Objeto(void);
	Objeto(pt::Point p,double orientacao);
	Objeto(Objeto&const o){
		this->posicao=o.posicao;
		this->orientacao=o.orientacao;
	}
	~Objeto(void);

	pt::Point posicao;
	double orientacao; //Angulo entre 0 a 2pi
};

