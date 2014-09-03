#pragma once
#include "stdafx.h"
#include "Objeto.h"
#include <cmath>
#include <iostream>
#include "includes.h"
using namespace std;
class Estrategia
{
public:
	Estrategia(void);
	Estrategia(Objeto *obj);
	~Estrategia(void);
	void switchFuzzy(float &defuzzy);
	pt::Point robo_longe_bola;
	pt::Point robo_perto_bola;
	pt::Point goleiroGoto;
protected:
private:
	void superDefesaEsquerda(float &defuzzy);
	void superDefesaMeio(float &defuzzy);
	void superDefesaDireita(float &defuzzy);
	void defesaEsquerda();
	void defesaMeio(float &defuzzy);
	void defesaDireita();
	void neutroEsquerda();
	void neutroMeio(float &defuzzy);
	void neutroDireita();
	void ataqueEsquerda(float &defuzzy);
	void ataqueMeio(float &defuzzy);
	void ataqueDireita(float &defuzzy);
	void superAtaqueDireita(float &defuzzy);
	void superAtaqueMeio(float &defuzzy);
	void superAtaqueEsquerda();
	void invertCampo(pt::Point *ponto);
	pt::Point roboMaisProximo(int robo);
	float distBolatoGoal();
	float diferencaRoboAdvRoboProximoeBola();
	float distanciaRoboProximoBola();
	bool indexRoboProximoBola();
	pt::Point goleiroPosition();
	void previsaoDaBola();
	void increaseArray();
	Objeto *objetos;
	bool roboProximoBola;
	pt::Point roboAdv; //roboAdvMaisProximodaBola
	pt::Point distanciaRoboAdv; //ro
	pt::Point caminho; //Caminho que A* calcula
	pt::Point path; //Pega o caminho de 3 em 3 pontos
};

