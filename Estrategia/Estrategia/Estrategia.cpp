#include "stdafx.h"
#include "Estrategia.h"


Estrategia::Estrategia(void)
{
	/*
	objetos[0] -[1] nosso robo
	objetos[2] nosso goleiro
	objetos[3] bola
	objetos[4] - [5] jogadores deles
	objetos[6] goleiro deles
	*/
}

Estrategia::Estrategia(Objeto *obj){
	this->objetos=obj;
	this->robo_longe_bola=pt::Point();
	this->robo_perto_bola=pt::Point();
	this->goleiroGoto=pt::Point();
	this->caminho=pt::Point(); //Caminho que A* calcula
	this->path=pt::Point(); //Pega o caminho de 3 em 3 pontos
	this->roboAdv=pt::Point(); //roboAdvMaisProximodaBola
	this->distanciaRoboAdv=pt::Point(); //ro
	this->roboProximoBola=false;
}

Estrategia::~Estrategia(void)
{
}


float x_array[3] = {0};
float y_array[3] = {0};

// previsao da bola
float Sx;
float Sy;
float Sx_y;
float b = 0;
float a = 0;
float x_mean = 0;
float y_mean = 0;
int length = sizeof (x_array) / sizeof (int);
float a_anterior = a;


void Estrategia::increaseArray() {
	if (x_array[length - 1] != objetos[3].posicao.getX() || y_array[length - 1] != objetos[3].posicao.getY()) {
		for (int i = 0; i < length - 1; i++) {
			x_array[i] = x_array[i + 1];
			y_array[i] = y_array[i + 1];
		}
		x_array[length - 1] = objetos[3].posicao.getX();
		y_array[length - 1] = objetos[3].posicao.getY();
	}
}

void Estrategia::previsaoDaBola() {
	bool limiar;

	if (CAMPO_INVERT)
		limiar = (objetos[3].posicao.getX() < 750) ? 1 : 0;
	else
		limiar = (objetos[3].posicao.getX() > 750) ? 1 : 0;

	for (int i = 0; i < length; i++) {
		x_mean += x_array[i];
		y_mean += y_array[i];
	}
	x_mean /= length;
	y_mean /= length;

	for (int i = 0; i < length; i++) {
		Sx += pow((x_array[i] - x_mean), 2);
		//      Sy+=pow((y_array[i]-y_mean),2);
		Sx_y += (x_array[i] - x_mean) * (y_array[i] - y_mean);
	}
	a = Sx_y / Sx;
	b = y_mean - a * x_mean;
	cout << "a: " << a << " b: " << b << endl;
	if (abs(a - a_anterior) < 10 && limiar && (a * goleiroGoto.getX() + b) > GOL_BOT_Y && (a * goleiroGoto.getX() + b) < GOL_TOP_Y) {
		this->goleiroGoto.setp(GOL_X, (a * goleiroGoto.getX()) + b);
	}
	a_anterior = a;
	Sx_y=0;
	Sx=0;
}


// Siga a bola
pt::Point Estrategia::goleiroPosition() {
	return pt::Point(GOL_X, objetos[3].posicao.getY());
}

// Compara as distancias dos meus robos com relacao bola
//indica qual o Objeto mais proximo � bola

bool Estrategia::indexRoboProximoBola() {
	float distancia0 = sqrt(pow(objetos[3].posicao.getX() - this->objetos[0].posicao.getX(), 2) + pow(objetos[3].posicao.getY() - this->objetos[0].posicao.getY(), 2));
	float distancia1 = sqrt(pow(objetos[3].posicao.getX() - this->objetos[1].posicao.getX(), 2) + pow(objetos[3].posicao.getY() - this->objetos[1].posicao.getY(), 2));

	return (distancia1 < distancia0) ? true : false;
}

float Estrategia::distanciaRoboProximoBola() {
	return sqrt(pow(objetos[3].posicao.getX() - this->objetos[roboProximoBola].posicao.getX(), 2) + pow(objetos[3].posicao.getY() - this->objetos[roboProximoBola].posicao.getY(), 2));
}

float Estrategia::diferencaRoboAdvRoboProximoeBola()// entrada ND do Fuzzy
{
	float distancia0 = sqrt(pow(objetos[3].posicao.getX() - this->objetos[4].posicao.getX(), 2) + pow(objetos[3].posicao.getY() - this->objetos[4].posicao.getY(), 2));
	float distancia1 = sqrt(pow(objetos[3].posicao.getX() - this->objetos[5].posicao.getX(), 2) + pow(objetos[3].posicao.getY() - this->objetos[5].posicao.getY(), 2));

	return ((distancia1 < distancia0) ? (distancia1 - distanciaRoboProximoBola()) : (distancia0 - distanciaRoboProximoBola()));
}

float Estrategia::distBolatoGoal()// entrada D do Fuzzy
{
	return sqrt(pow(objetos[3].posicao.getX() - GOL_X, 2) + pow(objetos[3].posicao.getY() - GOL_Y, 2));
}

// calcula a dist�ncia at� o Objeto adivers�rio mais pr�ximo

pt::Point Estrategia::roboMaisProximo(int robo) {
	float distancia0 = sqrt(pow(this->objetos[robo].posicao.getX() - this->objetos[4].posicao.getX(), 2) + pow(this->objetos[robo].posicao.getY() - this->objetos[4].posicao.getY(), 2));
	float distancia1 = sqrt(pow(this->objetos[robo].posicao.getX() - this->objetos[5].posicao.getX(), 2) + pow(this->objetos[robo].posicao.getY() - this->objetos[5].posicao.getY(), 2));

	return ((distancia1 < distancia0) ? pt::Point(this->objetos[4].posicao.getX(), this->objetos[4].posicao.getY()) : pt::Point(this->objetos[5].posicao.getX(), this->objetos[5].posicao.getY()));
}

void Estrategia::invertCampo(pt::Point *point) {
	point->setp(LARGURA_PISTA - point->getX(), ALTURA_PISTA - point->getY());
}

// -- A1

void Estrategia::superAtaqueEsquerda() {
	robo_longe_bola.setp(613.0, 280);
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());
}

// -- A2

void Estrategia::superAtaqueMeio(float &defuzzy) {
	//Bola mais a direita
	if (defuzzy < 6) {
		robo_longe_bola.setp(620, 1268);
		//Bola mais a esquerda
	} else {
		robo_longe_bola.setp(613.0, 280);

	}
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());
}

// -- A3

void Estrategia::superAtaqueDireita(float &defuzzy) {
	if (defuzzy < 6) {
		robo_longe_bola.setp(830, 900);
		//Bola mais a esquerda
	} else {
		robo_longe_bola.setp(620, 1268);
	}
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());
}

// -- B3

void Estrategia::ataqueDireita(float &defuzzy) {
	float decimalFuzzy = defuzzy - (int) (defuzzy);
	if (defuzzy < 4) {
		robo_longe_bola.setp(820 * (1 - decimalFuzzy), 1189);
	} else {
		robo_longe_bola.setp(830 * (1 - decimalFuzzy), 900 * (1 - decimalFuzzy));
	}
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());
}

// -- B2

void Estrategia::ataqueMeio(float &defuzzy) {

	float decimalFuzzy = defuzzy - (int) (defuzzy);
	if (defuzzy < 4) {
		robo_longe_bola.setp(830, 1268 * (1 + decimalFuzzy));
	} else {
		robo_longe_bola.setp(830, 280 * (1 - decimalFuzzy));
	}
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());
}

// -- B1

void Estrategia::ataqueEsquerda(float &defuzzy) {
	float decimalFuzzy = defuzzy - (int) (defuzzy);
	if (defuzzy < 2) {
		robo_longe_bola.setp(830, 1268 * (1 + decimalFuzzy));
		//Bola mais a esquerda
	} else {
		robo_longe_bola.setp(830, 280 * (1 + decimalFuzzy));
	}
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());

}

// -- C1

void Estrategia::neutroDireita() {
	// pt::Point distanciaRoboAdv = roboMaisProximo(!roboProximo);
	distanciaRoboAdv = roboMaisProximo(!roboProximoBola);
	robo_longe_bola.setp(distanciaRoboAdv.getX(), distanciaRoboAdv.getY() + 200);
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());

}

// -- C2

void Estrategia::neutroMeio(float &defuzzy) {
	float decimalFuzzy = defuzzy - (int) (defuzzy);
	//pt::Point distanciaRoboAdv = roboMaisProximo(!roboProximo);
	distanciaRoboAdv = roboMaisProximo(!roboProximoBola);
	if (decimalFuzzy > 0)
		robo_longe_bola.setp(distanciaRoboAdv.getX(), distanciaRoboAdv.getY() - 200);
	else
		robo_longe_bola.setp(distanciaRoboAdv.getX(), distanciaRoboAdv.getY() + 200);

	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());

}

// -- C3

void Estrategia::neutroEsquerda() {
	//pt::Point distanciaRoboAdv = roboMaisProximo(!roboProximo);
	distanciaRoboAdv = roboMaisProximo(!roboProximoBola);
	robo_longe_bola.setp(distanciaRoboAdv.getX(), distanciaRoboAdv.getY() - 200);
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());
}

// -- D3

void Estrategia::defesaDireita() {
	robo_longe_bola.setp(goleiroGoto.getX() - 150, goleiroGoto.getY() + 75);
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());
}

// -- D2

void Estrategia::defesaMeio(float &defuzzy) {

	if (defuzzy < -3) {
		robo_longe_bola.setp(goleiroGoto.getX() - 150, goleiroGoto.getY() - 75);
	} else {
		robo_longe_bola.setp(goleiroGoto.getX() - 150, goleiroGoto.getY() + 75);
	}
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());
}

// -- D1

void Estrategia::defesaEsquerda() {
	robo_longe_bola.setp(goleiroGoto.getX() - 150, goleiroGoto.getY() - 75);
	robo_perto_bola.setp(objetos[3].posicao.getX(), objetos[3].posicao.getY());
}

// -- E1

void Estrategia::superDefesaDireita(float &defuzzy) {
	float decimalFuzzy = 1;

	roboAdv = roboMaisProximo(3);

	if (defuzzy > -5)
		decimalFuzzy = defuzzy - (int) (defuzzy);

	robo_perto_bola.setp(roboAdv.getX() + cos((roboAdv.getY() - GOL_Y) / (roboAdv.getX() - LARGURA_PISTA)) * abs(decimalFuzzy) * 5, roboAdv.getX() + sin(abs(roboAdv.getY() - GOL_Y) / abs(roboAdv.getX() - LARGURA_PISTA)) * abs(decimalFuzzy) * 5);
	robo_longe_bola.setp(goleiroGoto.getX() - 150, goleiroGoto.getY() + 75);
}

// -- E2

void Estrategia::superDefesaMeio(float &defuzzy) {
	float decimalFuzzy = defuzzy - (int) (defuzzy);

	//pt::Point roboAdv = roboMaisProximo(3);
	roboAdv = roboMaisProximo(3);
	if (defuzzy > -6) {
		robo_perto_bola.setp(roboAdv.getX() + cos((roboAdv.getY() - GOL_Y) / (roboAdv.getX() - LARGURA_PISTA)) * abs(decimalFuzzy) * 5, roboAdv.getX() + sin(abs(roboAdv.getY() - GOL_Y) / abs(roboAdv.getX() - LARGURA_PISTA)) * abs(decimalFuzzy) * 5);
		robo_longe_bola.setp(goleiroGoto.getX() - 150, goleiroGoto.getY() + 75);

	} else {
		robo_perto_bola.setp(roboAdv.getX() + cos((roboAdv.getY() - GOL_Y) / (roboAdv.getX() - LARGURA_PISTA)) * abs(decimalFuzzy) * 5, roboAdv.getX() - sin(abs(roboAdv.getY() - GOL_Y) / abs(roboAdv.getX() - LARGURA_PISTA)) * abs(decimalFuzzy) * 5);
		robo_longe_bola.setp(goleiroGoto.getX() - 150, goleiroGoto.getY() - 75);

	}
}

// -- E3

void Estrategia::superDefesaEsquerda(float &defuzzy) {
	float decimalFuzzy = 1;

	//pt::Point roboAdv = roboMaisProximo(3);
	roboAdv = roboMaisProximo(3);
	if (defuzzy > -7)
		decimalFuzzy = defuzzy - (int) (defuzzy);

	robo_perto_bola.setp(roboAdv.getX() + cos((roboAdv.getY() - GOL_Y) / (roboAdv.getX() - LARGURA_PISTA)) * abs(decimalFuzzy) * 5, roboAdv.getX() - sin(abs(roboAdv.getY() - GOL_Y) / abs(roboAdv.getX() - LARGURA_PISTA)) * abs(decimalFuzzy) * 5);
	robo_longe_bola.setp(goleiroGoto.getX() - 150, goleiroGoto.getY() - 75);

}

void Estrategia::switchFuzzy(float &defuzzy) {
	cout << "GS: " << defuzzy << endl;
	switch ((int) (defuzzy + 0.5)) {
	case -8: //Verificar
	case -7:
		superDefesaDireita(defuzzy);
		break;
	case -6:
		superDefesaMeio(defuzzy);
		break;
	case -5:
		superDefesaEsquerda(defuzzy);
		break;
	case -4:
		defesaEsquerda();
		break;
	case -3:
		defesaMeio(defuzzy);
		break;
	case -2:
		defesaDireita();
		break;
	case -1:
		neutroDireita();
		break;
	case 0:
		neutroMeio(defuzzy);
		break;
	case 1:
		neutroEsquerda();
		break;
	case 2:
		ataqueEsquerda(defuzzy);
		break;
	case 3:
		ataqueMeio(defuzzy);
		break;
	case 4:
		ataqueDireita(defuzzy);
		break;
	case 5:
		superAtaqueDireita(defuzzy);
		break;
	case 6:
		superAtaqueMeio(defuzzy);
		break;
	case 7: //Verificar
	case 8:
		superAtaqueEsquerda();
		break;
	default:
		break;
	}
	if (!CAMPO_INVERT) {
		invertCampo(&robo_longe_bola);
		invertCampo(&robo_perto_bola);
	}
}
