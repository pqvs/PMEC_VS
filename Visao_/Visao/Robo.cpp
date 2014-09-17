#include "Robo.h"



Robo::Robo(const int id): id(id),Objeto()
{
	this->velocity_left=0;
	this->velocity_right=0;
	direcao = false;
}

Robo::Robo(const int id, pt::Point posicao, float orientacao):id(id), Objeto(posicao,orientacao)
{
	direcao = false;
}

Robo::~Robo(void)
{
}
