#include "InterfaceGrafica.h"

#define PI 3.14159265
#define COMPRIMENTO_CAMPO 148 //cm
#define LARGURA_CAMPO 127 //cm
#define BORDA_CAMPO 10 //cm
#define RAIO_ROBO (7.5f/2) //cm
#define TAMANHO_GOL 37 //cm
#define RAIO_BOLA 2.15f //cm
#define LARGURA_LINHA 10 //pixel memo
#define TAMANHO_HISTORICO 20 //u
#define TEMPO_FRAME 13 // ms

using namespace std;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
const float scale = FRAME_HEIGHT/(LARGURA_CAMPO); //pixels por cm

InterfaceGrafica::InterfaceGrafica(Objeto** objetos){
	this->objetos = objetos;
	Objeto* ball = this->objetos[3];
	pt::Point ballPosition = ball->posicao;
	this->lastBallPositions = new vector<pt::Point>(TAMANHO_HISTORICO);
	ballSpeed = 0;
	ballOrientation = PI;

	for(int i=0; i++; i<TAMANHO_HISTORICO){
		this->lastBallPositions->at(i) = pt::Point(ballPosition.x, ballPosition.y);
	}
}


InterfaceGrafica::~InterfaceGrafica(void){}

void InterfaceGrafica::iniciar(atomic<bool> *quit){
	while(quit->load() == false){
		Mat frame(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3, Scalar(0,0,0));
		
		desenharCampo(frame); 
		desenharObjetos(frame);
		//historicoPosicoesBola(frame);
		imshow("Interface", frame);
		resizeWindow("Interface", FRAME_WIDTH-111,FRAME_HEIGHT);
		waitKey(10);
	}
}

void InterfaceGrafica::desenharCampo(Mat frame){
	
	
	//Linhas laterais
	line(frame, Point((int)(BORDA_CAMPO*scale), (int)(BORDA_CAMPO*scale)), Point((int)((COMPRIMENTO_CAMPO+BORDA_CAMPO)*scale),(int)(BORDA_CAMPO*scale)), Scalar(255, 255, 255), 3, 8, 0);
	line(frame, Point((int)(BORDA_CAMPO*scale), (int)((LARGURA_CAMPO+BORDA_CAMPO)*scale)), Point((int)((COMPRIMENTO_CAMPO+BORDA_CAMPO)*scale),(int)((LARGURA_CAMPO+BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);

	//Linha de fundo Esquerda
	line(frame, Point((int)(BORDA_CAMPO*scale), (int)(BORDA_CAMPO*scale)),Point((int)(BORDA_CAMPO*scale),(int)(( (LARGURA_CAMPO/2) -(TAMANHO_GOL/2) + BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);
	line(frame, Point((int)(BORDA_CAMPO*scale), (int)(((LARGURA_CAMPO/2)+(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)),Point((int)((BORDA_CAMPO)*scale),(int)((LARGURA_CAMPO+BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);

	//Linha de fundo Direita
	line(frame, Point((int)((COMPRIMENTO_CAMPO+BORDA_CAMPO)*scale), (int)(BORDA_CAMPO*scale)),Point((int)((COMPRIMENTO_CAMPO+BORDA_CAMPO)*scale),(int)(( (LARGURA_CAMPO/2) -(TAMANHO_GOL/2) + BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);
	line(frame, Point((int)((COMPRIMENTO_CAMPO+BORDA_CAMPO)*scale), (int)(((LARGURA_CAMPO/2)+(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)),Point((int)((COMPRIMENTO_CAMPO+BORDA_CAMPO)*scale),(int)((LARGURA_CAMPO+BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);

	//Gol Esquerda
	line(frame, Point((int)((BORDA_CAMPO)*scale), (int)(((LARGURA_CAMPO/2)-(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)),Point(0,(int)(((LARGURA_CAMPO/2)-(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);
	line(frame, Point((int)((BORDA_CAMPO)*scale), (int)(((LARGURA_CAMPO/2)+(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)),Point(0,(int)(((LARGURA_CAMPO/2)+(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);
	line(frame, Point(0, (int)(((LARGURA_CAMPO/2)+(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)),Point(0,(int)(((LARGURA_CAMPO/2)-(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);


	//Gol Direita
	line(frame, Point((int)((COMPRIMENTO_CAMPO+BORDA_CAMPO)*scale), (int)(((LARGURA_CAMPO/2)-(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)),Point((int)((COMPRIMENTO_CAMPO+2*BORDA_CAMPO)*scale),(int)(((LARGURA_CAMPO/2)-(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);
	line(frame, Point((int)((COMPRIMENTO_CAMPO+BORDA_CAMPO)*scale), (int)(((LARGURA_CAMPO/2)+(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)),Point((int)((COMPRIMENTO_CAMPO+2*BORDA_CAMPO)*scale),(int)(((LARGURA_CAMPO/2)+(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);
	line(frame, Point((int)((COMPRIMENTO_CAMPO+2*BORDA_CAMPO)*scale), (int)(((LARGURA_CAMPO/2)+(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)),Point((int)((COMPRIMENTO_CAMPO+2*BORDA_CAMPO)*scale),(int)(((LARGURA_CAMPO/2)-(TAMANHO_GOL/2)+BORDA_CAMPO)*scale)), Scalar(255, 255, 255), 3, 8, 0);

}
void InterfaceGrafica::desenharObjetos(Mat frame){

float raioRobo = scale*RAIO_ROBO; //pixels

	//Desenha jogadores
	for(int i=0; i<3; i++){
		float deltaX = cos(objetos[i]->orientacao)*raioRobo;
		float deltaY = sin(objetos[i]->orientacao)*raioRobo;
		line(frame, Point((int)((objetos[i]->posicao.x)*scale+LARGURA_LINHA), (int)(objetos[i]->posicao.y+BORDA_CAMPO)*scale+LARGURA_LINHA), Point((int)((objetos[i]->posicao.x)*scale+deltaX+LARGURA_LINHA), (int)((objetos[i]->posicao.y+BORDA_CAMPO)*scale+deltaY+LARGURA_LINHA)), Scalar(255, 255, 255), 2, 8, 0);
		circle(frame, Point((int)((objetos[i]->posicao.x)*scale+LARGURA_LINHA),(int)(objetos[i]->posicao.y+BORDA_CAMPO)*scale+LARGURA_LINHA), (int)(raioRobo), Scalar(255, 255, 255), 2, 8, 0);
	}

	//Desenha bola
	circle(frame, Point((int)(objetos[3]->posicao.x)*scale+LARGURA_LINHA,(int)(objetos[3]->posicao.y+BORDA_CAMPO)*scale+LARGURA_LINHA), (int)(RAIO_BOLA*scale), Scalar(0,127,255), -1, 8, 0);

	//Desenha adversarios
	for(int i=4; i<7; i++){
		circle(frame, Point((int)((objetos[i]->posicao.x)*scale+LARGURA_LINHA),(int)(objetos[i]->posicao.y+BORDA_CAMPO)*scale+LARGURA_LINHA), (int)(raioRobo), Scalar(0, 0, 255), 2, 8, 0);
	}
}

void InterfaceGrafica::historicoPosicoesBola(Mat frame){

	Objeto* ball = this->objetos[3];
	pt::Point ballPosition = ball->posicao;
	int metadeHist = ((int)TAMANHO_HISTORICO/2);
	double medianaXNova[((int)TAMANHO_HISTORICO/2)];
	double medianaYNova[((int)TAMANHO_HISTORICO/2)];

	double medianaXAntiga[((int)TAMANHO_HISTORICO/2)];
	double medianaYAntiga[((int)TAMANHO_HISTORICO/2)];

	medianaXNova[0] = ballPosition.x;
	medianaXNova[0] = ballPosition.y;

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

	double deltaSBX = ballPosition.x-medianaXAntiga[((int)metadeHist/2)], deltaSBY = ballPosition.y-medianaYAntiga[((int)metadeHist/2)];

	double vbx = deltaSBX*1000/(TAMANHO_HISTORICO*TEMPO_FRAME), vby = deltaSBY*1000/(TAMANHO_HISTORICO*TEMPO_FRAME);
	
	ballOrientation = atan2(deltaSBY,deltaSBX);
	ballSpeed = sqrt(pow(vbx, 2) + pow(vby, 2));
	

	if(ballOrientation <0 && ballOrientation>=-PI/2)
		ballOrientation+=PI;
	else if(ballOrientation <=PI/2 && ballOrientation >= 0)
		ballOrientation-= PI;

	line(frame, Point((int)((objetos[3]->posicao.x)*scale+LARGURA_LINHA), (int)(objetos[3]->posicao.y+BORDA_CAMPO)*scale), Point((int)((objetos[3]->posicao.x)*scale+deltaSBX+LARGURA_LINHA), (int)((objetos[3]->posicao.y+BORDA_CAMPO)*scale+deltaSBY)), Scalar(255, 255, 255), 2, 8, 0);
	
	pt::Point futureBallPos;
	futureBallPos.x = ballPosition.x + TEMPO_FRAME*ballSpeed*cos(ballOrientation)/1000;
	futureBallPos.y = ballPosition.y + TEMPO_FRAME*ballSpeed*sin(ballOrientation)/1000;

	//circle(frame, Point((int)(futureBallPos.x)*scale+LARGURA_LINHA,(int)(futureBallPos.y+BORDA_CAMPO)*scale), (int)(RAIO_BOLA*scale), Scalar(0,255,0), -1, 8, 0);
	double yoBall = tan(ballOrientation)*(BORDA_CAMPO-futureBallPos.x)+futureBallPos.y;
	circle(frame, Point((int)(BORDA_CAMPO)*scale+LARGURA_LINHA,(int)(yoBall+BORDA_CAMPO)*scale), (int)(RAIO_BOLA*scale), Scalar(255,0,0), -1, 8, 0);

}