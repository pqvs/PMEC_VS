#include "Tag.h"
#include "define.h"


Tag::Tag(string nome, int H_MIN, int H_MAX, int S_MIN, int S_MAX, int V_MIN, int V_MAX){
	this->nome = nome;
	this->H_MAX = H_MAX;
	this->H_MIN = H_MIN;
	this->S_MAX = S_MAX;
	this->S_MIN = S_MIN;
	this->V_MAX = V_MAX;
	this->V_MIN = V_MIN;
	this->A_MIN = 140;
	this->threshold.create(cv::Size(FRAME_WIDTH,FRAME_HEIGHT), 0);
	trackWindow = cv::Rect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);

	posicoes = new vector<pt::Point>(3);

	this->aH = new int[256];
	this->aS = new int[256];
	this->aV = new int[256];
}

Tag::~Tag(void){
}

void Tag::setHSV( int H_MIN, int H_MAX, int S_MIN, int S_MAX, int V_MIN, int V_MAX, int A_MIN){
	this->H_MAX = H_MAX;
	this->H_MIN = H_MIN;
	this->S_MAX = S_MAX;
	this->S_MIN = S_MIN;
	this->V_MAX = V_MAX;
	this->V_MIN = V_MIN;
	this->A_MIN = A_MIN;

	for(int i = 0; i<256; i++){
		// H
		if(i >= H_MIN && i <= H_MAX)
			this->aH[i] = 1;
		else
			this->aH[i] = 0;
		// S
		if(i >= S_MIN && i <= S_MAX)
			this->aS[i] = 1;
		else
			this->aS[i] = 0;
		// V
		if(i >= V_MIN && i <= V_MAX)
			this->aV[i] = 1;
		else
			this->aV[i] = 0;
	}

}