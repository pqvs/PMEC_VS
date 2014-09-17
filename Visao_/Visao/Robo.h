#pragma once
#include "Objeto.h"
class Robo :
	public Objeto
{

public:
	Robo();
	Robo(const int id);
	Robo(const int id, pt::Point posicao, float orientacao);
	virtual ~Robo(void);
	int velocity_right,velocity_left;
	const int id;
	bool direcao;
};

