//#define _CRT_SECURE_NO_WARNINGS
#include "Visao.h"
#include <thread>
#include <atomic>
#include "InterfaceGrafica.h"
#include "EEGoalKeeper.h"
#include "stdafx.h"
#include "Objeto.h"
#include "SerialW.h"
#include "ControladorPID.h"
#include "EstrategiaGoleiro.h"
#include "Robo.h"

double pcFreq = 0.0;
__int64 counterstart = 0;
atomic<bool> quit (false);

static Objeto** objetos;

static ControladorPID** controladores;

void interfaceGrafica();

void estrategiaGoleiro();

int main(int argc, char* argv[]){
	objetos = new Objeto*[7];
	controladores = new ControladorPID*[3];
	thread threads[2];
	Visao *visao = NULL;

	for(int i=0;i<7;i++){
		if(i!=3){
			objetos[i] = new Robo(i);
			if(i<3)
				controladores[i] = new ControladorPID(1, 0.001, 0.1, ((Robo*)objetos[i]));
		}else
			objetos[i] = new Bola();
	}
	visao = new Visao(objetos);

	threads[0] = thread(interfaceGrafica);
	threads[1] = thread(estrategiaGoleiro);
	visao->iniciar(); 
	quit.store(true);
 
	for (auto& th : threads) th.join();

	return 0;
}

void interfaceGrafica(){
	InterfaceGrafica *interfaceGrafica = new InterfaceGrafica(objetos);
	interfaceGrafica->iniciar(ref(quit));
}

void estrategiaGoleiro(){
	SerialW s= SerialW("\\\\.\\COM30");
	Robo* goalkeeper = ((Robo*)objetos[0]);
	Bola* bola = ((Bola*)objetos[3]);
	double kp=3, ki=0.001, kd=0.16;

	getchar();
	EEGoalKeeper eeGoalkeeper(objetos);
	EstrategiaGoleiro estGol(goalkeeper, bola);

	int*pwmGoalkeeper	= new int[2];
	pwmGoalkeeper[0]	= 0;
	pwmGoalkeeper[1]	= 0;
	bool flag			= false;

	((Robo*)objetos[1])->velocity_left=3;
	((Robo*)objetos[1])->velocity_right=4;
	((Robo*)objetos[2])->velocity_left=5;
	((Robo*)objetos[2])->velocity_right=6;
	

	float lastX=0, lastY =0;
	double ganhoEsq = 1;
	while(true){

		if(_kbhit()){
			char ch = _getch();
			switch(ch){
				case VK_SPACE:
					flag =!flag;
					cout << "PAUSE COMUNICACAO: " << flag <<endl;
					controladores[0]->resetConst(kp, ki, kd);
					break;
				case '1':
					kp+=0.1;
					cout <<"kp: "<< kp <<endl;
					break;
				case '2':
					ki+=0.0001;
					cout <<"ki: "<< ki <<endl;
					break;
				case '3':
					kd+=0.01;
					cout <<"kd: "<< kd <<endl;
					break;
				case '4':
					kp-=0.1;
					cout <<"kp: "<< kp <<endl;
					break;
				case '5':
					ki-=0.0001;
					cout <<"ki: "<< ki <<endl;
					break;
				case '6':
					kd-=0.01;
					cout <<"kd: "<< kd <<endl;
					break;
				case 'q':
					pwmGoalkeeper[0]++;
					cout <<"pwmGoalkeeper esq: "<< pwmGoalkeeper[0] <<endl;
					break;
				case 'w':
					pwmGoalkeeper[0]--;
					cout <<"pwmGoalkeeper esq: "<< pwmGoalkeeper[0] <<endl;
					break;
				case 'e':
					pwmGoalkeeper[1]++;
					cout <<"pwmGoalkeeper dir: "<< pwmGoalkeeper[1] <<endl;
					break;
				case 'r':
					pwmGoalkeeper[1]--;
					cout <<"pwmGoalkeeper dir: "<< pwmGoalkeeper[1] <<endl;
					break;
				case '7':
					ganhoEsq+=0.1;
					cout <<"ganhoEsq: "<< ganhoEsq <<endl;
					break;
				case '8':
					ganhoEsq-=0.1;
					cout <<"ganhoEsq: "<< ganhoEsq <<endl;
					break;
				case '0':
					((Robo*)objetos[0])->velocity_left = 60;
					((Robo*)objetos[0])->velocity_right = -60;  
					s.send(((Robo*)objetos[0]),((Robo*)objetos[1]),((Robo*)objetos[2]));
					cout <<"Vira..."<<endl;
					break;
			}
			controladores[0]->setConst(kp, ki, kd);
		}
		if(flag){
			
			//----------------------------GOLEIRO DO TIAGUERA COM PID---------------------//
			/*estGol.predicaoDaBola();
			estGol.estrategia();*/
			//---------------------------------------------------------------------------//
			
			//-----------------------------EE APOLO---------------------------------------//
			eeGoalkeeper.evolve(pwmGoalkeeper);
			goalkeeper->velocity_left = abs(pwmGoalkeeper[0])<PWM_MAX ? pwmGoalkeeper[0] : PWM_MAX*pwmGoalkeeper[0]/abs(pwmGoalkeeper[0]);
			goalkeeper->velocity_right = abs(pwmGoalkeeper[1])<PWM_MAX ? pwmGoalkeeper[1] : PWM_MAX*pwmGoalkeeper[1]/abs(pwmGoalkeeper[1]);
			//---------------------------------------------------------------------------//

			//--------------------------------PID ANTIGO--------------------------------//
			/*double anguloAlvo = atan2(objetos[0]->posicao.y-objetos[3]->posicao.y, objetos[0]->posicao.x-objetos[3]->posicao.x);
            
			if(anguloAlvo <0 && anguloAlvo>=-PI/2)
				anguloAlvo+=PI;
			else if(anguloAlvo <=PI/2 && anguloAlvo >= 0)
				anguloAlvo-= PI;*/
			//cout<< "angulo: "<< anguloAlvo <<endl;
				//objetos[0]->orientacao + RAIO_RODA_ROBO*(pwmGoalkeeper[0]-pwmGoalkeeper[1])*VEL_MAX/(PWM_MAX*RAIO_ROBO*2);
			//double velocidadeLinear = 5; 

				//(pwmGoalkeeper[0] + pwmGoalkeeper[1])*VEL_MAX*RAIO_RODA_ROBO/(2*PWM_MAX);
			
			//ORIGINAL
			//controladores[0]->PID2(objetos[3]->posicao.x,objetos[3]->posicao.y);
			
			//CORRIGIDO
			//controladores[0]->PID(objetos[3]->posicao.x,objetos[3]->posicao.y);
			
			//FEITO PELO APOLEIRA
			//controladores[0]->execute(objetos[0]->orientacao,anguloAlvo,velocidadeLinear,pwmGoalkeeper);
			//---------------------------------------------------------------------------//
			
			goalkeeper->velocity_left = abs(goalkeeper->velocity_left)<PWM_MAX ? goalkeeper->velocity_left : PWM_MAX*goalkeeper->velocity_left/abs(goalkeeper->velocity_left);
			goalkeeper->velocity_right = abs(goalkeeper->velocity_right)<PWM_MAX ? goalkeeper->velocity_right : PWM_MAX*goalkeeper->velocity_right/abs(goalkeeper->velocity_right);

			s.send(((Robo*)objetos[0]),((Robo*)objetos[1]),((Robo*)objetos[2]));
		}
	}
}