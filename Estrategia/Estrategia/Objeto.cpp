#include "stdafx.h"
#include "Objeto.h" 


Objeto::Objeto(void)
{
}

Objeto::Objeto(pt::Point p,float orientacao)
{
	this->posicao=p;
	this->orientacao=orientacao;
}

Objeto::~Objeto(void)
{
}
