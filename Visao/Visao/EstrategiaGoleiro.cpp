#include "EstrategiaGoleiro.h"

EstrategiaGoleiro::EstrategiaGoleiro(Robo *robo, Bola *bola){
	this->robo = robo;
	this->bola = bola;
	this->controlador = new ControladorPID(0, 0, 0, this->robo);

	this->lastPositions = new vector<pt::Point>(TAMANHO_HISTORICO);
	this->lastBallPositions = new vector<pt::Point>(TAMANHO_HISTORICO);

	for(int i=0; i++; i<TAMANHO_HISTORICO){
		this->lastPositions->at(i) = pt::Point(robo->posicao.x,  robo->posicao.y);
		this->lastBallPositions->at(i) = pt::Point(bola->posicao.x, bola->posicao.y);
	}

}

EstrategiaGoleiro::~EstrategiaGoleiro(){}

void EstrategiaGoleiro::estrategia(){

	double VxBola = bola->velocity*cos(bola->orientacao);
	double yFuturo= 0;
	if(VxBola<0){
		yFuturo = tan(bola->orientacao)*(bola->posicao.x - BORDA_CAMPO+10) + bola->posicao.y;
	}else{
		yFuturo = bola->posicao.y;
	}

	if(yFuturo<(LARGURA_CAMPO-TAMANHO_GOL)/2)
		yFuturo = (LARGURA_CAMPO-TAMANHO_GOL)/2;

	if(yFuturo > (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL)
		yFuturo = (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL;
	//
	controlador->pidTiaguera(pt::Point(BORDA_CAMPO+10, yFuturo), robo->posicao, robo->orientacao);
}

void EstrategiaGoleiro::predicaoDaBola(void){

	pt::Point ballPosition(this->bola->posicao.x,this->bola->posicao.y);

	int metadeHist = ((int)TAMANHO_HISTORICO/2);
	double medianaXNova[((int)TAMANHO_HISTORICO/2)];
	double medianaYNova[((int)TAMANHO_HISTORICO/2)];

	double medianaXAntiga[((int)TAMANHO_HISTORICO/2)];
	double medianaYAntiga[((int)TAMANHO_HISTORICO/2)];

	//Atualiza as posicoes antigas
	for(int i=(TAMANHO_HISTORICO-1); i>0; i--){
		lastBallPositions->at(i).x = lastBallPositions->at(i-1).x;
		lastBallPositions->at(i).y = lastBallPositions->at(i-1).y;

		if(i<metadeHist){
			medianaXNova[i] = lastBallPositions->at(i).x;
			medianaYNova[i] = lastBallPositions->at(i).y;
		}else{
			medianaXAntiga[i-metadeHist] = lastBallPositions->at(i).x;
			medianaYAntiga[i-metadeHist] = lastBallPositions->at(i).y;
		}
	}

	lastBallPositions->at(0).x = ballPosition.x;
	lastBallPositions->at(0).y = ballPosition.y;


	//Calcula a Mediana dos valores antigos e dos novos valores//
	medianaXNova[0] = ballPosition.x;
	medianaXNova[0] = ballPosition.y;
	double auxX, auxY;  // variavel auxiliar.
   
	for (int i = 0; i < (metadeHist - 1); i++){
		for (int j = i + 1; j < metadeHist; j++){
			if (medianaXNova[i] > medianaXNova[j])
			{
				auxX = medianaXNova[i];
				medianaXNova[i] = medianaXNova[j];
				medianaXNova[j] = auxX;
			}

			if (medianaYNova[i] > medianaYNova[j])
			{
				auxY = medianaYNova[i];
				medianaYNova[i] = medianaYNova[j];
				medianaYNova[j] = auxY;
			}

			if (medianaXAntiga[i] > medianaXAntiga[j])
			{
				auxX = medianaXAntiga[i];
				medianaXAntiga[i] = medianaXAntiga[j];
				medianaXAntiga[j] = auxX;
			}

			if (medianaYAntiga[i] > medianaYAntiga[j])
			{
				auxY = medianaYAntiga[i];
				medianaYAntiga[i] = medianaYAntiga[j];
				medianaYAntiga[j] = auxY;
			}
		}
	}
	ballPosition.x = medianaXNova[((int)metadeHist/2)];
	ballPosition.y = medianaYNova[((int)metadeHist/2)];
	//----------------------------------------------//

	//Calculo de velocidade e orientacao da bola
	double	deltaSBX = ballPosition.x-medianaXAntiga[((int)metadeHist/2)], 
			deltaSBY = ballPosition.y-medianaYAntiga[((int)metadeHist/2)];
	double vbx = deltaSBX*1000/(TAMANHO_HISTORICO*TEMPO_FRAME), vby = deltaSBY*1000/(TAMANHO_HISTORICO*TEMPO_FRAME);
	
	bola->orientacao = atan2(deltaSBY,deltaSBX);
	bola->velocity = sqrt(pow(vbx, 2) + pow(vby, 2));

	if(bola->orientacao <0 && bola->orientacao>=-PI/2)
		bola->orientacao+=PI;
	else if(bola->orientacao <=PI/2 && bola->orientacao >= 0)
		bola->orientacao-= PI;
}
