//#define _CRT_SECURE_NO_WARNINGS
#include "Visao.h"
#include <thread>
#include "InterfaceGrafica.h"
#include "EEGoalKeeper.h"
#include "stdafx.h"
#include "Objeto.h"
#include "SerialW.h"
#include "ControladorPID.h"
#include "EstrategiaGoleiro.h"
#include "Robo.h"
#include "FuzzyVS2014.h"
#include "EEMidForward.h"

//bool re=false;
double pcFreq = 0.0;
__int64 counterstart = 0;
float angcirc=0;
float xcirc , ycirc;
static Objeto** objetos; //s�o os objetos rastreados. S�o compartilhados com a vis�o interface e estrat�gia; 0 goleiro 1 e 2 jogadores 3 bola
static ControladorPID** controladores;
static FuzzyVS2014** fuzzyController;//fuzzy s� vai precisar de ter um pra cada robo se tivermos que customizar as parti��es ou base de regras, se n�o pode deixar com o objeto est�tico n�o precisa ser **
void StartCounter();
double GetCounter();
bool side = true;
bool lastStuck = false;
float kp=0.5, ki=0.003, kd=0.5;//PID tem que ter um pra cada robo!!!
bool batida=false;
std::atomic<int> ready (3);

void interfaceGrafica(){
	InterfaceGrafica *interfaceGrafica = new InterfaceGrafica(objetos);
	interfaceGrafica->iniciar();
}


void robot_controller(Robo *player){
	Robo*robo=player;
	
	while(true){

		while (robo->ready) { 
			
		//cout<<robo->id<<" is stuck "<<robo->ready<<endl;
		std::this_thread::yield(); }      // wait for the ready signal
		//cout<<robo->id<<" is stuck"<<endl;
		float xr=robo->posicao.x;
		float yr=robo->posicao.y;
		float angulorobo=robo->orientacao;
		int ve;
		int vd;
		
		float Ot=atan2((robo->target.y-yr),(robo->target.x-xr));
		float dist=sqrt(pow((robo->target.y-yr),2)+pow((robo->target.x-xr),2));
		Ot=atan2(sin(angulorobo-Ot),cos(angulorobo-Ot));
		
		if(Ot>=-PI/2 && Ot<=PI/2){
			robo->re=false;
		}
		else {
			robo->re=true;
			Ot=Ot+PI;
			Ot=atan2(sin(Ot),cos(Ot));
		}

		Ot=-Ot*180/PI;
		if(robo->id==0)
		cout<<robo->id<<" id "<<Ot<<" Ot"<<endl;
		//convers�o de centimetros para milimetros
		dist*=10;
		
		//bool check=robo->checa_batida();
		//cout<<"check : "<<check<<endl;
		if(!robo->checa_batida()){
		
			vector<float> velocidades = fuzzyController[0]->ControladorFuzzy(Ot,dist);//velocidades[0]=esquerda velocidades[1]=direita


			if (robo->re){
				robo->velocity_left=-(velocidades[1]*kd+40);
				robo->velocity_right=-(velocidades[0]*kp+40);

			}else{
				robo->velocity_left=(velocidades[0]*kd+40);
				robo->velocity_right=(velocidades[1]*kp+40);
			}	
		}
		robo->ready=true;
	
		
		}
}

void estrategiaGoleiro(){
	SerialW s= SerialW("\\\\.\\COM30");
	Robo* goalkeeper = ((Robo*)objetos[0]);
	Robo* ojuara = ((Robo*)objetos[1]);
	Robo* lenhador = ((Robo*)objetos[2]);
	Bola* bola = ((Bola*)objetos[3]);
	goalkeeper->id=0;
	ojuara->id=1;
	lenhador->id=2;
	
	std::cout << "Spawning robot controllers threads...\n";
	std::thread t1 (robot_controller,goalkeeper);
	std::thread t2 (robot_controller,ojuara);
	std::thread t3 (robot_controller,lenhador);
	std::cout << "Done spawning threads. Now waiting for them to join:\n";

	getchar();
	//EEGoalKeeper eeGoalkeeper(objetos);
	//EEMidForward eeOjuara(objetos);
	EstrategiaGoleiro estGol(goalkeeper, bola);

	int*pwmGoalkeeper = new int[2];
	pwmGoalkeeper[0] = 0;
	pwmGoalkeeper[1] = 0;
	bool flag=false;
	

	//ojuara->velocity_left=0;
	//ojuara->velocity_right=0;
	//lenhador->velocity_left=0;
	//lenhador->velocity_right=0;


	float lastX=0, lastY =0;
	double ganhoEsq = 1;
	float i=0;
	while(true){
		goalkeeper->histFeed(goalkeeper->posicao.x,goalkeeper->posicao.y);

		if(kbhit()){//regula cosntantes PID

			char ch = getch();
			switch(ch){
			case VK_SPACE:
				flag =!flag;
				cout << "PAUSE COMUNICACAO: " << flag <<endl;
				controladores[0]->resetConst(kp, ki, kd);
				break;
			case '*':
				batida=!batida;//isso daqui vai ter que mudar pq n�o vai funcionar;
				cout <<"Batida: "<< batida <<endl;
				break;
			case '1':
				kp+=0.1;
				cout <<"kp: "<< kp <<endl;
				break;
			case '2':
				ki+=0.001;
				cout <<"ki: "<< ki <<endl;
				break;
			case '3':
				kd+=0.1;
				cout <<"kd: "<< kd <<endl;
				break;
			case '4':
				kp-=0.1;
				cout <<"kp: "<< kp <<endl;
				break;
			case '5':
				ki-=0.001;
				cout <<"ki: "<< ki <<endl;
				break;
			case '6':
				kd-=0.1;
				cout <<"kd: "<< kd <<endl;
				break;
			case '-':
				cout <<"trocou "<<endl;
				side=!side;
				if(side){
					xcirc=80;
					ycirc=0;
				}else{

					xcirc=80;
					ycirc=120;
				}

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
				/*	case '7':
				ganhoEsq+=0.1;
				cout <<"ganhoEsq: "<< ganhoEsq <<endl;
				break;
				case '8':
				ganhoEsq-=0.1;
				cout <<"ganhoEsq: "<< ganhoEsq <<endl;
				break;*/
			case '0':
				goalkeeper->velocity_left = 60.0f;
				goalkeeper->velocity_right = -60.0f;  
				s.send(goalkeeper,ojuara,lenhador);
				cout <<"Vira..."<<endl;
				break;
			case '7':
				goalkeeper->velocity_left = 60.0f;
				goalkeeper->velocity_right = 60.0f;  
				ojuara->velocity_left = 0.0f;
				ojuara->velocity_right = 0.0f; 
				lenhador->velocity_left = 0.0f;
				lenhador->velocity_right = 0.0f; 
				s.send(goalkeeper,ojuara,lenhador);
				cout <<"Reto..."<<endl;
				break;
			case '8':
				ojuara->velocity_left = 60.0f;
				ojuara->velocity_right = 60.0f; 
				goalkeeper->velocity_left = 0.0f;
				goalkeeper->velocity_right = 0.0f; 
				lenhador->velocity_left = 0.0f;
				lenhador->velocity_right = 0.0f; 
				s.send(goalkeeper,ojuara,lenhador);
				cout <<"Reto..."<<endl;
				break;
			case '9':
				lenhador->velocity_left = 60.0f;
				lenhador->velocity_right = 60.0f; 
				ojuara->velocity_left = 0.0f;
				ojuara->velocity_right = 0.0f; 
				goalkeeper->velocity_left = 0.0f;
				goalkeeper->velocity_right = 0.0f; 
				s.send(goalkeeper,ojuara,lenhador);
				cout <<"Reto..."<<endl;
				break;

			}
			controladores[0]->setConst(kp, ki, kd);
		}

		if(flag){
			//bola->posicao.setX(xcirc);
			//bola->posicao.setY(ycirc);
			goalkeeper->target.setp(bola->posicao);
			
			//cout<<goalkeeper->target.x<<"  "<<goalkeeper->target.y<<endl;
			
			////n�o � o melhor jeito, mas vamos testar pra ver no que d�.
		//	eeOjuara.evolve(pwmGoalkeeper);
			//eeGoalkeeper.evolve(pwmGoalkeeper);
		//goalkeeper->velocity_left = abs(pwmGoalkeeper[0])<PWM_MAX ? pwmGoalkeeper[0] :   PWM_MAX*pwmGoalkeeper[0]/abs(pwmGoalkeeper[0]);
		//goalkeeper->velocity_right = abs(pwmGoalkeeper[1])<PWM_MAX ? pwmGoalkeeper[1] : PWM_MAX*pwmGoalkeeper[1]/abs(pwmGoalkeeper[1]);
		
			//cout<<"VE1 : "<<goalkeeper->velocity_left <<" -  VD1 : "<<goalkeeper->velocity_right <<endl;

			//if(goalkeeper->velocity_left!=0)
			//	goalkeeper->velocity_left = kp*(abs(goalkeeper->velocity_left)/goalkeeper->velocity_left)*(abs(goalkeeper->velocity_left)+30);
		//	if(goalkeeper->velocity_right!=0)
			//	goalkeeper->velocity_right = kd*(abs(goalkeeper->velocity_right)/goalkeeper->velocity_right)*(abs(goalkeeper->velocity_right)+30);
			//cout<<"VE : "<<goalkeeper->velocity_left <<" -  VD : "<<goalkeeper->velocity_right <<endl;
			
		
			goalkeeper->ready=false;//reseta as flags de controle
			ojuara->ready=false;
			lenhador->ready=false;

			while((!goalkeeper->ready
				&& !ojuara->ready
				&& !lenhador->ready)){
					
					
					//esperar todos os robos acabarem
					//cout<<goalkeeper->id<<" is stuck "<<goalkeeper->ready<<endl;

			}
			
			s.send(goalkeeper,ojuara,lenhador);
			Sleep(10);
		}	


	}
	t1.join();
	t2.join();
	t3.join();
	
	//std::cout << "All threads joined!\n";
}


int main(int argc, char* argv[]){
	objetos = new Objeto*[7];

	controladores = new ControladorPID*[3];
	for(int i=0;i<7;i++){
		if(i!=3){
			objetos[i]=new Robo(i);
			if(i<3)
				controladores[i] = new ControladorPID(1, 0.001, 0.1, ((Robo*)objetos[i]));
		}else
			objetos[i]=new Bola();
	}

	Visao *visao = new Visao(objetos);

	thread interfaceGrafica(interfaceGrafica);
	thread estrategiaGoleiro(estrategiaGoleiro);
	visao->iniciar(); 


	return 0;
}


//#include <iostream>           // std::cout
//#include <thread>             // std::thread
//#include <mutex>              // std::mutex, std::unique_lock
//#include <condition_variable> // std::condition_variable
//
//using namespace std;
//
//mutex mtx;
//condition_variable cv;
//bool break_code= false;
//
//void print_thread (int id) {
//
//	//objeto lock para travar o codigo ate acordar
//  unique_lock<std::mutex> lck(mtx);
//  while (!break_code){ 
//	  //Trava para o evento de trigger
//	  cv.wait(lck);
//
//	  //Colocar o codigo a ser executado
//	  cout << "thread " << id << '\n';
//  }
//}
//
//
//int main ()
//{
//  thread threads[10];
//  // spawn 10 threads:
//  for (int i=0; i<10; ++i)
//    threads[i] = thread(print_thread,i);
//
//  cout << "10 threads ready to race...\n";
//
//
//  //Loop principal
//  while(!break_code){
//	  char c = getchar();
//	  //Chamada de metodo de tracking (coloquei um evento de digitar g)
//	  if(c=='g'){
//		cv.notify_all();     //Acorda threads
//	  }else if(c=='x'){
//		  break_code = true;
//		  cv.notify_all();
//	  }
//  }
//
//  //Nem chega nesse join pq tem loop infinito hueBR
//  for (auto& th : threads) th.join();
//
//  return 0;
//}


void StartCounter(){
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	pcFreq = double(li.QuadPart)/1000.0;

	QueryPerformanceCounter(&li);
	counterstart = li.QuadPart;
}
double GetCounter(){
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart-counterstart)/pcFreq;
}