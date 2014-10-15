#include "EEMidForward.h"
#include <iostream>

EEMidForward::EEMidForward(Objeto** objetos)
{
	this->objetos = objetos;
	ball = (Bola*)(objetos[3]);
	robot = (Robo*)(objetos[0]);
	enemies = (Robo**)(&objetos[4]);
	allies = (Robo**)(&objetos[1]);

	robotPosition = pt::Point(objetos[0]->posicao.x,objetos[0]->posicao.y);
	pt::Point ballPosition(ball->posicao.x, ball->posicao.y);
	
	this->lastPositions = new vector<pt::Point>(TAMANHO_HISTORICO);
	this->lastBallPositions = new vector<pt::Point>(TAMANHO_HISTORICO);

	for(int i=0; i++; i<TAMANHO_HISTORICO){
		this->lastPositions->at(i) = pt::Point(robotPosition.x,  robotPosition.y);
		this->lastBallPositions->at(i) = pt::Point(ballPosition.x, ballPosition.y);
	}
	this->pwm = new int[2];
	this->pwm[0] = 0;
	this->pwm[1] = 0;

	this->sigma = 0.9*PWM_MAX;
	//this->fitness = evaluateFitness(this->pwm);
	this->robot = robot;
	this->ball = ball;
	this->updateVars();
	this->speed = 0;
	this->ballOrientation = PI;
	this->ballSpeed = 0;
	this->sumErroLinear = 0;
	this->sumErroAngular = 0;
	this->ballIn=false;
}


EEMidForward::~EEMidForward(void)
{

}

void EEMidForward::resetVars(){
	ballOrientation = PI;
	ballSpeed = 0;
	pt::Point ballPosition(ball->posicao.x, ball->posicao.y);

	for(int i=0; i<TAMANHO_HISTORICO; i++){
		lastBallPositions->at(i).x = ballPosition.x;
		lastBallPositions->at(i).y = ballPosition.y;
	}
}

void EEMidForward::updateVars(void){

	//Copia para variavel local
	robotPosition = pt::Point(objetos[0]->posicao.x, objetos[0]->posicao.y);

	//Copia orientacao
	robotOrientation = objetos[0]->orientacao;

	//CALCULO DE VELOCIDADE DO ROBO//
	double deltaSRX = robotPosition.x-lastPositions->at(0).x, 
		   deltaSRY = robotPosition.y-lastPositions->at(0).y;
	for(int i=1; i<TAMANHO_HISTORICO; i++){
		deltaSRX += lastPositions->at(i).x - lastPositions->at(i-1).x;
		deltaSRY += lastPositions->at(i).y - lastPositions->at(i-1).y;
	}
	speed = sqrt(pow(deltaSRX*1000/(TAMANHO_HISTORICO*TEMPO_FRAME), 2) + pow(deltaSRY*1000/(TAMANHO_HISTORICO*TEMPO_FRAME), 2));
	//----------------------------------------//

	//Atualiza as posicoes antigas
	for(int i=(TAMANHO_HISTORICO-1); i>0; i--){
		lastPositions->at(i).x = lastPositions->at(i-1).x;
		lastPositions->at(i).y = lastPositions->at(i-1).y;
	}
	lastPositions->at(0).x = robotPosition.x;
	lastPositions->at(0).y = robotPosition.y;
	//---------------------------------------//

	//Verifica se esta com a bola
	ballIn = (EEMidForward::distancePoints(ball->posicao, robotPosition) <= (RAIO_ROBO+5));

	//CAMPO POTENCIAL//
	//calcPotField();
	//---------------//
}

void EEMidForward::calcPotField(){

	//Atualiza os valores do campo
	initializeField();
	//-------------------//

	double erro;
	double p=0.5*(cos(PI/FIELD_SIZE)+cos(PI/FIELD_SIZE));
	double w=2/(1+sqrt(1-pow(p,2)));
	double campoIJ = 0;
	double erroMinimo = 10E-6;
	double eanterior = 0, eatual;
	int t;
 
	for (t = 0; t < 5000; t++){
		eatual = 0;
		for (int i = 1; i < FIELD_SIZE-1; i++){
			for (int j = 1; j < FIELD_SIZE-1; j++){
				// Obstáculo ou meta
				if(potField[i][j] == 1 || potField[i][j] == 0)
					continue;
			
				potField[i][j] = potField[i][j]+w*(potField[i][j-1]+potField[i][j+1]+potField[i-1][j]+potField[i+1][j]-4*potField[i][j])/4;
				eatual = eatual + abs(potField[i][j]-oldPotField[i][j]);
			}
		}
		if(abs(eatual - eanterior) < 10.0E-12){
			break;
		}

		// Copia o valor de campo para campoOld
		for (int i = 0; i < FIELD_SIZE; i++) {
			for (int j = 0; j < FIELD_SIZE; j++) {
				oldPotField[i][j] = potField[i][j];
			}
		}  
		eanterior = eatual;
	}
	
	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			oldPotField[i][j] = potField[i][j];
		}
	}  
	//Calcula o divergente da inversa em coordenada polar
	double mTop, mBot, mEsq, mDir;
	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			mTop = (i-1) > 0 ? oldPotField[i-1][j] : 1;
			mBot = (i+1) < FIELD_SIZE-1 ? oldPotField[i+1][j] : 1;
			mEsq = (j-1) >0 ? oldPotField[i][j-1] : 1;
			mDir = (j+1) < FIELD_SIZE-1 ? oldPotField[i][j+1] : 1;
			potField[i][j] = atan2((mEsq-mDir),(mTop-mBot));
		}
	}
	
	calcRepulsionField();
}

void EEMidForward::calcRepulsionField(){
	pt::Point rPos(robotPosition.x, robotPosition.y);
	pt::Point ballPos(this->ball->posicao.x, this->ball->posicao.y);

	//Altera o campo dos inimigos para um campo giratorio
	for(int e=4; e<7; e++){
		if(enemiesNear[e-4]){
			pt::Point enPos(this->objetos[e]->posicao.x, this->objetos[e]->posicao.y);
			int xObsI = (int)(((enPos.x-RAIO_ROBO)-(rPos.x-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE);
			int xObsF = (int)(((enPos.x+RAIO_ROBO)-(rPos.x-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE);
			int yObsI = (int)(((enPos.y-RAIO_ROBO)-(rPos.y-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE);
			int yObsF = (int)(((enPos.y+RAIO_ROBO)-(rPos.y-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE);
			double teta= atan2(FIELD_SIZE/2-(yObsF-RAIO_ROBO/(CELL_SIZE)),FIELD_SIZE/2-(xObsF-RAIO_ROBO/(CELL_SIZE)));

			for(int i=xObsI-1; i< xObsF+1; i++){
				for(int j=yObsI-1; j<yObsF+1; j++){
            
            
					
					//Nao altera o campo em torno da meta
					if(distancePoints(goalPos, pt::Point(i,j))*CELL_SIZE>2*RAIO_ROBO){
						double val = atan2(j-(yObsF-RAIO_ROBO/(CELL_SIZE)),i-(xObsF-RAIO_ROBO/(CELL_SIZE)))-PI/2;
            

						if((teta<=PI/2 && teta>=0)|| (teta<=-PI/2 && teta>=-PI)){
							potField[i][j]=atan2(sin(val+PI), cos(val+PI));
						}else{
							potField[i][j]=val;
						}
					}
            
				}
			}
		}
	}

	//Altera o campo da meta para pegar a bola por traz
	int def=BALL_RANGE_DEF, delta=0;
	//(rPos.x<ballPos.x+(def-delta)&&rPos.x>ballPos.x-(def-delta) && rPos.y<ballPos.y+(def-delta)  && rPos.y>ballPos.y-(def-delta)
	if( (distancePoints(rPos, ballPos)<CELL_SIZE*(def-delta)) && !ballIn){
		//cout<<"atraz"<<endl;
		int xbi=((ballPos.x-(rPos.x-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE)-def;
		
		if(xbi<0)
			xbi=0;

		int xbf=((ballPos.x-(rPos.x-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE)+def;
		if(xbi>FIELD_SIZE-1)
			xbi=FIELD_SIZE-1;
	
		int ybi=((ballPos.y-(rPos.y-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE)-def;
		if(ybi<0)
			ybi=0;
	
		int ybf=((ballPos.y-(rPos.y-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE)+def;
		if(ybf>FIELD_SIZE-1)
			xbi=FIELD_SIZE-1;
    
		for (int i=xbi; i<xbf; i++){
			for(int j=ybi; j<ybf; j++){
				//Corredor para pegar a bola por traz
				if(!(i>xbi+def && ((j==ybi+def)||j==ybi+def+1||j==ybi+def-1||j==ybi+def-2||j==ybi+def+2))){
					double val = atan2(j-(ybf-def),i-(xbf-def))-PI/2;
					/*cout<<xbi<<endl;
					cout<<ybi<<endl;
					getchar();*/
					/*if(j == ybi+2){
						if(i == xbi||i == xbi+1){
							val = atan2(j-(ybf-def),i-(xbf-def))-8*PI/18;
						}else if (i == xbf||i == xbf-1){
							val = atan2(j-(ybf-def),i-(xbf-def))-PI/2;
						}
					}*/
				
					double teta = atan2(FIELD_SIZE/2-j,FIELD_SIZE/2-i);
					/*if ((i==xbi||i==xbf||j==ybi||j==ybf||i==xbi+1||i==xbf-1||j==ybi+1||j==ybf-1) || (i>xbi+def && ((j==ybi+def) || j==ybi+def+1 || j==ybi+def-1))){
						val = atan2(sin(val)-sin(potField[i][j]),cos(val)-cos(potField[i][j]));
					}*/
					if((teta<=PI/2 && teta>=0)|| (teta<=-PI/2 && teta>=-PI )){
						//cout<<"if"<<endl;
						potField[i][j]=atan2(sin(val+PI), cos(val+PI));
						
					}else{
						//cout<<"else"<<endl;
						potField[i][j]=val;
					}
				}
			}
		}
	}
	fieldOrientation = potField[FIELD_SIZE/2][FIELD_SIZE/2];

}

void EEMidForward::initializeField(){

	pt::Point rPos(robotPosition.x, robotPosition.y);
	
	bool eParede=false;
	double offsetX=0, offsetY=0;
	for (int i = 0; i < FIELD_SIZE; i++){
		offsetX = ((rPos.x-FIELD_SIZE/2)+i);
      for (int j = 0; j < FIELD_SIZE; j++){
		  potField[i][j]=0.5;
		  offsetY = ((rPos.y-FIELD_SIZE/2)+j);
		  eParede = ((offsetX<=BORDA_CAMPO)&&(offsetY  > (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL || offsetY < (LARGURA_CAMPO-TAMANHO_GOL)/2))
			  || ((offsetX>=COMPRIMENTO_CAMPO+BORDA_CAMPO)&&(offsetY  > (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL || offsetY < (LARGURA_CAMPO-TAMANHO_GOL)/2))
			  || (offsetY<=0) 
			  || (offsetY>=LARGURA_CAMPO);
		  if(i==0 || j==0 || i==FIELD_SIZE-1 || j==FIELD_SIZE-1 || eParede)
			   potField[i][j]=1;
      }
    }

	for(int e=4; e<7; e++){
		enemiesNear[e]=false;
		pt::Point enPos(this->objetos[e]->posicao.x, this->objetos[e]->posicao.y);

		//CALCULO DE DISTANCIA DOS ROBOS INIMIGOS//
		if(EEMidForward::distancePoints(rPos, enPos)<=(FIELD_SIZE)*CELL_SIZE/2){
			enemiesNear[e-4]=true;

			int xObsI = ((enPos.x-RAIO_ROBO)-(rPos.x-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE;
			int xObsF = ((enPos.x+RAIO_ROBO)-(rPos.x-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE;
			int yObsI = ((enPos.y-RAIO_ROBO)-(rPos.y-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE;
			int yObsF = ((enPos.y+RAIO_ROBO)-(rPos.y-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE;

			if(xObsI<0)
				xObsI=0;
			if(xObsI>FIELD_SIZE-1)
				xObsI=FIELD_SIZE-1;

			if(xObsF<0)
				xObsF=0;
			if(xObsF>FIELD_SIZE-1)
				xObsF=FIELD_SIZE-1;

			if(yObsI<0)
				yObsI=0;
			if(yObsI>FIELD_SIZE-1)
				yObsI=FIELD_SIZE-1;
		    
			if(yObsF<0)
				yObsF=0;
			if(yObsF>FIELD_SIZE-1)
				yObsF=FIELD_SIZE-1;

			//Ocupa as celulas do campo
			for(int i=xObsI; i<=xObsF; i++){
				for(int j=yObsI; j<=yObsF; j++){
					potField[i][j]=1;
				}
			}
		}
	}

	//Se estiver com a posse de bola
	if (ballIn){
		//A meta é o gol
			goalPos=pt::Point((int)((BORDA_CAMPO-(rPos.x-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE), (int)(((LARGURA_CAMPO/2)-(rPos.y-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE));
			realGoalPos = pt::Point(BORDA_CAMPO, (LARGURA_CAMPO/2));
	}else{
		//A meta é a bola
		goalPos=pt::Point((int)((ball->posicao.x-(rPos.x-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE), (int)((ball->posicao.y-(rPos.y-FIELD_SIZE*CELL_SIZE/2))/CELL_SIZE));
		realGoalPos = pt::Point(ball->posicao.x,ball->posicao.y);
	}
	//Se a meta estiver dentro do campo o ponto de atracao fica 4 celulas atras da meta
	if(EEMidForward::distancePoints(goalPos, pt::Point((FIELD_SIZE/2),(FIELD_SIZE/2)))>=(FIELD_SIZE)/2 - 4){
		double angle = atan2((goalPos.x-FIELD_SIZE/2),(goalPos.y-FIELD_SIZE/2));
		goalPos.x = (int)(((FIELD_SIZE-4)*sin(angle)/2)+FIELD_SIZE/2);
		goalPos.y = (int)(((FIELD_SIZE-4)*cos(angle)/2)+FIELD_SIZE/2);
	}

	for (int i = goalPos.x-1; i <= goalPos.x+1; i++){
      for (int j = goalPos.y-1; j < goalPos.y+1; j++){

		  potField[i][j]=0.5;
	  }
	}
	potField[(int)(goalPos.x)][(int)(goalPos.y)] = 0;
	//getchar();
}

void EEMidForward::evolve(int* pwm){
	//Atualiza o erro da ultima iteracao
	//calcErro();
	//Atualiza as variáveis
	updateVars();

	int *child = new int[2];
	this->fitness = evaluateFitness(this->pwm);
	
	totalMutations = 0;
	positiveMutations = 0;
	this->sigma = 0.9*PWM_MAX;

	for(int g=0; g<1000; g++){
		child[0] = this->pwm[0];
		child[1] = this->pwm[1];
		applyMutation(child);
	
		this->totalMutations++;

		double childFitness = evaluateFitness(child);
		if(fitness<childFitness){
			this->pwm[0] = child[0];
			this->pwm[1] = child[1];
			this->fitness = childFitness;
			this->positiveMutations++;
		}

		//if(totalMutations%200 == 0)
		//	updateSigma();
		
	}
	delete[] child;
	pwm[0] = this->pwm[0];
	pwm[1] = this->pwm[1];
	
	//CORRECAO DO ERRO DO ULTIMO FRAME//
	//this->pwm[1] += (this->erroLinear - RAIO_ROBO*this->erroAngular)/(RAIO_RODA_ROBO);
	//this->pwm[0] += (this->erroLinear + RAIO_ROBO*this->erroAngular)/(RAIO_RODA_ROBO);
	//-------------------------------------------//
	
	if(abs(pwm[1])< PWM_MIN){
		pwm[1] = 0;
	}
	if(abs(pwm[0])< PWM_MIN){
		pwm[0] = 0;
	}

	calcExpected(this->pwm);

	//cout<< this->fitness  <<endl;
	//cout <<"pwm0: "<< pwm[0]<<" pwm1: "<<pwm[1]<<" fitness: "<< fitness <<" meta: "<<goalPos.x<<","<<goalPos.y<<" robo: "<<robotPosition.x<<","<<robotPosition.y<<endl;
	//cout << pwm[0] <<"\t" <<pwm[1]<<endl;

}

void EEMidForward::calcExpected(int pwm[2]){
	double tetaR = atan2(sin(robotOrientation), cos(robotOrientation));
	pt::Point rPos(robotPosition.x, robotPosition.y);
	
	if(abs(pwm[0])< PWM_MIN){
		pwm[0] = 0;
	}
	if(abs(pwm[1])< PWM_MIN){
		pwm[1] = 0;
	}

	tetaR += RAIO_RODA_ROBO*(pwm[0]-pwm[1])*VEL_MAX_FRAME/(RAIO_ROBO*2*PWM_MAX);
	
	tetaR = atan2(sin(tetaR), cos(tetaR));

	robotExpectedSpeed = abs(RAIO_RODA_ROBO*(pwm[0]+pwm[1])*VEL_MAX_FRAME/(2*PWM_MAX));
	rPos.y += RAIO_RODA_ROBO*(pwm[0]+pwm[1])*sin(tetaR)*VEL_MAX_FRAME/(2*PWM_MAX);
    rPos.x += RAIO_RODA_ROBO*(pwm[0]+pwm[1])*cos(tetaR)*VEL_MAX_FRAME/(2*PWM_MAX);
	//Bater na parede
	if((rPos.x < BORDA_CAMPO+LARGURA_LINHA+RAIO_ROBO && (rPos.y > (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL || rPos.y < (LARGURA_CAMPO-TAMANHO_GOL)/2))){
		expectedRobotPos.x = BORDA_CAMPO+LARGURA_LINHA+RAIO_ROBO ;	
	}else{
		expectedRobotPos.x = rPos.x;
	}
	expectedRobotPos.y = rPos.y;
	expectedOri = tetaR;
	expectedDist = distancePoints(rPos,  robotPosition);
}

/**
*	REALIMENTACAO DO SISTEMA
*/
void EEMidForward::calcErro(){
	double tetaR = atan2(sin(robotOrientation), cos(robotOrientation));
	pt::Point rPos(robotPosition.x, robotPosition.y);

	float oriE = (expectedOri - tetaR);
	oriE = atan2(sin(oriE), cos(oriE));
	float oriEre;

	if (tetaR > PI)
		oriEre = (expectedOri - (tetaR - PI));
	else
		oriEre = (expectedOri - (tetaR + PI));

	oriEre = atan2(sin(oriEre), cos(oriEre));

	bool re = false;
	if (abs(oriEre) < abs(oriE)) {
		oriE = oriEre;
		re = true;
	} 

	float dist = distancePoints( lastPositions->at(0), lastPositions->at(1));
	
	this->sumErroLinear += expectedDist-dist;
	this->sumErroAngular += oriE;

	double KpW=1, KiW=0.001, KpV=0.1, KiV=0.0001;

	this->erroAngular = KpW * oriE + this->sumErroAngular*KiW;
	this->erroLinear = KpV * (expectedDist-dist) + this->sumErroLinear*KiV;

	/*if(re)
		this->erroLinear = -this->erroLinear;*/
}

double  EEMidForward::evaluateFitness(int* child){

	double tetaR = atan2(sin(robotOrientation), cos(robotOrientation));
	
	pt::Point rPos(robotPosition.x, robotPosition.y);

	//Calcula a posicao e orientacao esperada
	calcExpected(child);
	
	/**
		Pot_Field
		Ot=O+(W_Size*(speed(1)-speed(2))/R_Size)*180/pi;
		vr=W_Size*(speed(1)+speed(2))/2;
		yt=yr+W_Size*(speed(1)+speed(2))*sind(Ot)/2;
		xt=xr+W_Size*(speed(1)+speed(2))*cosd(Ot)/2;
		Om=(atan2(yt-yr,xt-xr)*180/pi);
		fitness=(1000*cosd(Om-orientation)+10*abs(vr))/(1000+sqrt((goal(2)-yt)^2+(goal(1)-xt)^2));
	*/
		
		float dist=sqrt(pow((robot->target.y-expectedRobotPos.y),2)+pow((robot->target.x-expectedRobotPos.x),2));
//		Ot=atan2(sin(angulorobo-Ot),cos(angulorobo-Ot));
	//Om - Orientacao do vetor resultante do movimento
	//double Om = atan2((expectedRobotPos.y-rPos.y), (expectedRobotPos.x-rPos.x));
	//double expectedDistGoal = sqrt(pow(((goalPos.y*CELL_SIZE+rPos.y)-expectedRobotPos.y), 2)+pow(((goalPos.x*CELL_SIZE+rPos.x)-expectedRobotPos.x),2));
	float Om=atan2((robot->target.y-expectedRobotPos.y),(robot->target.x-expectedRobotPos.x));
	//std::cout<<"Om : "<<Om*180/PI<<std::endl;
	double childFitness = (abs(pow(cos(Om-expectedOri),2)))/(100+pow(dist,2));
	//double childFitness = 1000*(10*pow(cos(Om-fieldOrientation),2)+2*abs(robotExpectedSpeed));
	//childFitness/=(1+1*distancePoints(realGoalPos, expectedRobotPos));
	/*if(!ballIn)
		childFitness/=(1+1*distancePoints(ball->posicao, expectedRobotPos));*/
	/*if(child[0]==0 && child[1]==0){
		cout<<Om<<" , "<<cos(Om-fieldOrientation)<<endl;
		getchar();
	}*/
	return childFitness;
}

void EEMidForward::applyMutation(int* child){
	normal_distribution<double> distribution(0,sigma);
	
	double valor0 = distribution(generator);
	double valor1 = distribution(generator);
	child[0] += valor0;
	child[1] += valor1;

	if(abs(child[0])>PWM_MAX)
		child[0] = PWM_MAX*child[0]/abs(child[0]);

	if(abs(child[1])>PWM_MAX)
		child[1] = PWM_MAX*child[1]/abs(child[1]);

	//cout<< "valor 0: " << valor0 << " valor1: "<< valor1 << endl;
}

void EEMidForward::updateSigma(void){
	if(totalMutations>0){
		if((positiveMutations/totalMutations)>0.2){
			sigma*=CI;
		}else if((positiveMutations/totalMutations)<0.2){
			sigma*=CD;
		}
	}

	if(sigma>PWM_MAX)
		sigma=PWM_MAX;

}

double EEMidForward::distancePoints(pt::Point A, pt::Point B){
	return sqrt(pow((A.x-B.x),2) + pow((A.y-B.y),2));
}