#pragma once
#ifndef ESTRATEGIA_GOLEIRO_H

#define ESTRATEGIA_GOLEIRO_H

#include "define.h"
#include "Robo.h"
#include "Bola.h"
#include "ControladorPID.h"
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;
class EstrategiaGoleiro
{
private:
	Robo *robo;
	Bola *bola;
	ControladorPID *controlador;
	vector<pt::Point> *lastPositions, *lastBallPositions;

public:
	EstrategiaGoleiro(Robo *robo, Bola *bola);
	~EstrategiaGoleiro();
	void estrategia();
	void predicaoDaBola(void);

};
#endif