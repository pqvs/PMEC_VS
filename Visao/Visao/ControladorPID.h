#pragma once


#include "define.h"
#include "Robo.h"
#include <math.h>
#include <iostream>

const double KpV=3, KiV=0.0001, KdV=0, KpW=3, KiW=0.0001, KdW=0;
using namespace std;
class ControladorPID
{
private:
	double	Kp, Ki, Kd, erroAcumulado, erroAnterior, xAnterior, yAnterior,
		erroAcumuladoV, erroAnteriorV,
		erroAcumuladoW, erroAnteriorW;
	Robo* robo;
public:
	ControladorPID();
	ControladorPID(double Kp, double Ki, double Kd, Robo *robo);
	void PID2(double xb, double yb);
	void resetConst(double Kp, double Ki, double Kd);
	void setConst(double kp, double ki, double kd);
	void execute(double anguloAtual, double anguloAlvo, double velocidadeLinear, int *pwm);
	void PID(double xb, double yb);
	void pidTiaguera(pt::Point alvo, pt::Point atual, double orientacaoAtual);
	~ControladorPID(void);
};
