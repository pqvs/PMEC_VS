#pragma once
#include "Objeto.h"
#include <math.h>
#include <stdio.h>

#define MAX_HIST 100
#define NUMBER_OF_CHECA_BATIDA 20
class Robo :
	public Objeto
{

public:
	Robo();
	Robo(const int id);
	Robo(const int id, pt::Point posicao, float orientacao);
	void histFeed(float x,float y);
	void histWipe();
	bool checa_batida();
	virtual ~Robo(void);
	int velocity_right,velocity_left;
	int id;
	bool direcao;
	bool stuck;
	int lastPos;
	float historico[2][MAX_HIST];
	float result;
	float printable;
	bool re;
	bool ready;
	pt::Point target;

private:
	bool is_Stuck();
	int count_checa_batida;

};

