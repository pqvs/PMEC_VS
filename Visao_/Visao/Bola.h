#pragma once
#include "objeto.h"
class Bola :
	public Objeto
{
public:
	Bola(void);
	Bola(pt::Point posicao, float orientacao);
	~Bola(void);
	float velocity;
};

