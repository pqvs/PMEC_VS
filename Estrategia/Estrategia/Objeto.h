#pragma once
#include "Point.h"
class Objeto
{
public:
	Objeto(void);
	Objeto(pt::Point p,float orientacao);
	Objeto(const Objeto &o){
		this->posicao=o.posicao;
		this->orientacao=o.orientacao;
	}
	~Objeto(void);

	pt::Point posicao;
	float orientacao; //Angulo entre 0 a 2pi
};

