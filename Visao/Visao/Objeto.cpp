#include "stdafx.h"
#include "Objeto.h" 


Objeto::Objeto(void)
{
	this->posicao=pt::Point(0,0);
	this->orientacao=0;
}

Objeto::Objeto(pt::Point p,double orientacao)
{
	this->posicao=p;
	this->orientacao=orientacao;
}

Objeto::~Objeto(void)
{
}
