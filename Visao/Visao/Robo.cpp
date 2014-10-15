#include "Robo.h"
#include <iostream>


Robo::Robo(const int id): id(id),Objeto()
{   
	this->velocity_left=0;
	this->velocity_right=0;
	direcao = false;
	lastPos=0;
	this->stuck=false;
	this->count_checa_batida=NUMBER_OF_CHECA_BATIDA;
	this->re=false;
	this->ready=true;

}
void Robo::histFeed(float px,float py){

	this->historico[0][this->lastPos]=px;
	this->historico[1][this->lastPos]=py;
	lastPos++;
	if(lastPos==MAX_HIST){
		lastPos=0;
	}
}
void Robo::histWipe(){
	for (int i=0;i<MAX_HIST;i++)
	{

		this->historico[0][i]=0;
		this->historico[1][i]=0;

	}


}

bool Robo::checa_batida(){
	//std::cout<<" count_checa_batida : "<<this->count_checa_batida<<std::endl;
	if(this->count_checa_batida>0&&this->count_checa_batida<NUMBER_OF_CHECA_BATIDA){
		this->count_checa_batida--;
		return true;
	}
	else { 
		if(this->is_Stuck()){
			this->count_checa_batida--;
			//cout<<"To preso carai"<<"    -   result : "<< goalkeeper->result<<endl;
			if(this->re){
				this->velocity_left=60;
				this->velocity_right=60;


			}//colocar as velocidade para o caso do robo bater de ré
			else{
				this->velocity_left=-60;
				this->velocity_right=-60;

			}//colocar as velocidade para o caso do robo bater de frente

			//goalkeeper->velocity_left = -ve/abs(ve)*80;
			//goalkeeper->velocity_right = -vd/abs(vd)*60;
			//s.send(goalkeeper,ojuara,lenhador,50,800,50);	
			return true;
		}else{
			this->count_checa_batida=NUMBER_OF_CHECA_BATIDA;
			return false;
		}
	}
}
bool Robo::is_Stuck(){
	//std::cout<<"lastPos : "<<lastPos<<std::endl;
	if(lastPos<=MAX_HIST/2)
		return false;
	float meanx=0;
	float meany=0;
	float stddivx=0;
	float stddivy=0;
	result = 0;


	for(int i=0;i<MAX_HIST;i++){
		meanx+=historico[0][i];
		meany+=historico[1][i];
	}

	meanx/=MAX_HIST;
	meany/=MAX_HIST;

	for(int i=0;i<MAX_HIST;i++){
		stddivx+=pow((historico[0][i]-meanx),2);
		stddivy+=pow((historico[1][i]-meany),2);
	}

	stddivx=sqrt(stddivx/MAX_HIST);
	stddivy=sqrt(stddivy/MAX_HIST);

	this->result=sqrt(pow(stddivx,2)+pow(stddivy,2));

	//std::cout<<" result : "<<this->result<<std::endl;
	if (abs(result)<0.3)
		return true;
	else 
		return  false;


}
Robo::Robo(const int id, pt::Point posicao, float orientacao):id(id), Objeto(posicao,orientacao)
{

	direcao = false;
	this->count_checa_batida=NUMBER_OF_CHECA_BATIDA;
	this->re=false;
	this->ready=true;

}

Robo::~Robo(void)
{

}
