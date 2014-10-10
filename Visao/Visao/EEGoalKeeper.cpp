#include "EEGoalKeeper.h"


EEGoalKeeper::EEGoalKeeper(Objeto** objetos)
{
	ball = (Bola*)(objetos[3]);
	robot = (Robo*)(objetos[0]);
	enemies = (Robo**)(&objetos[4]);
	allies = (Robo**)(&objetos[1]);

	pt::Point robotPosition(robot->posicao.x,robot->posicao.y);
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
}


EEGoalKeeper::~EEGoalKeeper(void)
{

}

void EEGoalKeeper::resetVars(){
	ballOrientation = PI;
	ballSpeed = 0;
	pt::Point ballPosition(ball->posicao.x, ball->posicao.y);

	for(int i=0; i<TAMANHO_HISTORICO; i++){
		lastBallPositions->at(i).x = ballPosition.x;
		lastBallPositions->at(i).y = ballPosition.y;
	}
}

void EEGoalKeeper::updateVars(void){

	//CALCULO DE VELOCIDADE DO ROBO//
	pt::Point robotPosition = pt::Point(robot->posicao.x, robot->posicao.y);

	double deltaSRX = robotPosition.x-lastPositions->at(0).x, 
		   deltaSRY = robotPosition.y-lastPositions->at(0).y;
	for(int i=1; i<TAMANHO_HISTORICO; i++){
		deltaSRX += lastPositions->at(i).x - lastPositions->at(i-1).x;
		deltaSRY += lastPositions->at(i).y - lastPositions->at(i-1).y;
	}
	speed = sqrt(pow(deltaSRX*1000/(TAMANHO_HISTORICO*TEMPO_FRAME), 2) + pow(deltaSRY*1000/(TAMANHO_HISTORICO*TEMPO_FRAME), 2));
	//Atualiza as posicoes antigas
	for(int i=(TAMANHO_HISTORICO-1); i>0; i--){
		lastPositions->at(i).x = lastPositions->at(i-1).x;
		lastPositions->at(i).y = lastPositions->at(i-1).y;
	}
	lastPositions->at(0).x = robotPosition.x;
	lastPositions->at(0).y = robotPosition.y;
	//---------------------------------------//

	//PREDICAO DA BOLA//
	predicaoDaBola();
	//------------------//

	//CALCULO DE DISTANCIA DOS DEMAIS ROBOS//
	pt::Point enemie0Pos(enemies[0]->posicao.x, enemies[0]->posicao.y);
	pt::Point enemie1Pos(enemies[1]->posicao.x, enemies[1]->posicao.y);
	pt::Point enemie2Pos(enemies[2]->posicao.x, enemies[2]->posicao.y);
	
	pt::Point allie0Pos(enemies[0]->posicao.x, allies[0]->posicao.y);
	pt::Point allie1Pos(enemies[1]->posicao.x, allies[1]->posicao.y);

	distanceEnemie0 = EEGoalKeeper::distancePoints(enemie0Pos, lastBallPositions->at(0));
	distanceEnemie1 = EEGoalKeeper::distancePoints(enemie1Pos, lastBallPositions->at(0));
	distanceEnemie2 = EEGoalKeeper::distancePoints(enemie2Pos, lastBallPositions->at(0));
	distanceAllie0 = EEGoalKeeper::distancePoints(allie0Pos, lastBallPositions->at(0));
	distanceAllie1 = EEGoalKeeper::distancePoints(allie1Pos, lastBallPositions->at(0));
	distanceGoalkeeper = EEGoalKeeper::distancePoints(robotPosition, lastBallPositions->at(0));
	distanceFromGoal = EEGoalKeeper::distancePoints(pt::Point(BORDA_CAMPO, (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL/2), pt::Point( lastBallPositions->at(0).x, (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL/2));
	//Distancia do jogador inimigo mais proximo
	distanceCloserEnemie = (distanceEnemie0<distanceEnemie1 ? ( distanceEnemie0<distanceEnemie2 ? distanceEnemie0 : distanceEnemie2) : ( distanceEnemie1<distanceEnemie2 ? distanceEnemie1 : distanceEnemie2)  );
	//------------------------------------//
}
void EEGoalKeeper::predicaoDaBola(void){

	pt::Point ballPosition(this->ball->posicao.x,this->ball->posicao.y);

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
	
	ballOrientation = atan2(deltaSBY,deltaSBX);
	ballSpeed = sqrt(pow(vbx, 2) + pow(vby, 2));

	if(ballOrientation <0 && ballOrientation>=-PI/2)
		ballOrientation+=PI;
	else if(ballOrientation <=PI/2 && ballOrientation >= 0)
		ballOrientation-= PI;

	//Numero de frames para previsao
	int numeroFrames = TAMANHO_HISTORICO/2;
	this->futureBallPos.x = ballPosition.x + numeroFrames*TEMPO_FRAME*ballSpeed*cos(ballOrientation)/1000;
	this->futureBallPos.y = ballPosition.y + numeroFrames*TEMPO_FRAME*ballSpeed*sin(ballOrientation)/1000;

	this->yFinalBall = tan(ballOrientation)*(BORDA_CAMPO-futureBallPos.x)+futureBallPos.y;
}

void EEGoalKeeper::evolve(int* pwm){
	//Atualiza o erro da ultima iteracao
	calcErro();
	//Atualiza as variáveis
	updateVars();

	int *child = new int[2];
	this->fitness = evaluateFitness(this->pwm);
	
	totalMutations = 0;
	positiveMutations = 0;
	this->sigma = 0.9*PWM_MAX;

	for(int g=0; g<800; g++){
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

		if(totalMutations%20 == 0)
			updateSigma();
		
	}
	delete[] child;
	pwm[0] = this->pwm[0];
	pwm[1] = this->pwm[1];
	
	//CORRECAO DO ERRO DO ULTIMO FRAME//
	this->pwm[1] += (this->erroLinear - RAIO_ROBO*this->erroAngular)/(RAIO_RODA_ROBO);
	this->pwm[0] += (this->erroLinear + RAIO_ROBO*this->erroAngular)/(RAIO_RODA_ROBO);
	//-------------------------------------------//
	
	if(abs(pwm[1])< PWM_MIN){
		pwm[1] = 0;
	}
	if(abs(pwm[0])< PWM_MIN){
		pwm[0] = 0;
	}

	calcExpected(this->pwm);

	//cout<< this->fitness  <<endl;
	//cout <<"pwm0: "<< pwm[0]<<" pwm1: "<<pwm[1]<<" fitness: "<< fitness <<endl;
	//cout << pwm[0] <<"\t" <<pwm[1]<<endl;
}

void EEGoalKeeper::calcExpected(int pwm[2]){
	double tetaR = atan2(sin(this->robot->orientacao), cos(this->robot->orientacao));
	pt::Point rPos(this->robot->posicao.x, this->robot->posicao.y);
	
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
	expectedDist = distancePoints(rPos,  this->robot->posicao);
}

/**
*	REALIMENTACAO DO SISTEMA
*/
void EEGoalKeeper::calcErro(){
	double tetaR = atan2(sin(this->robot->orientacao), cos(this->robot->orientacao));
	pt::Point rPos(this->robot->posicao.x, this->robot->posicao.y);

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

	double KpW =0.7,KiW=0.005,KpV=0.3,KiV=0.004;
	float dist = distancePoints( lastPositions->at(0), lastPositions->at(1));

	this->sumErroLinear+=expectedDist-dist;
	this->sumErroAngular+=oriE;

	this->erroAngular = KpW*oriE+this->sumErroAngular*KiW;
	this->erroLinear = KpV*(expectedDist-dist)+this->sumErroLinear*KiV;



	/*if(re)
		this->erroLinear = -this->erroLinear;*/
}

double  EEGoalKeeper::evaluateFitness(int* child){
	
	double tetaR = atan2(sin(this->robot->orientacao), cos(this->robot->orientacao));
	pt::Point rPos(this->robot->posicao.x, this->robot->posicao.y);

	//Calcula a posicao e orientacao esperada
	calcExpected(child);
	
	//Verifica se deve seguir o y caso longe ou parar na trajetoria da bola
	double yoBall = (distanceGoalkeeper<30) ? this->yFinalBall : this->futureBallPos.y;

	if(yoBall<(LARGURA_CAMPO-TAMANHO_GOL)/2)
		yoBall = (LARGURA_CAMPO-TAMANHO_GOL)/2;

	if(yoBall > (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL)
		yoBall = (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL;

	
	double distanceYo = EEGoalKeeper::distancePoints(expectedRobotPos, pt::Point(BORDA_CAMPO+RAIO_ROBO+LARGURA_LINHA,yoBall));
	double childFitness = 1000/pow(distanceYo, 2);
	//double childFitness = 1/distanceYo;
	double deltaGolY = 10, deltaGolX = 20;

	//Evitar de afastar do gol no X
	if(expectedRobotPos.x > BORDA_CAMPO+deltaGolX +LARGURA_LINHA){
		childFitness-=abs(50*(expectedRobotPos.x-BORDA_CAMPO+deltaGolX +LARGURA_LINHA));
	}else if((expectedRobotPos.x < BORDA_CAMPO+LARGURA_LINHA)){
		childFitness-=abs(1000*(expectedRobotPos.x-BORDA_CAMPO-LARGURA_LINHA));	
	}
	
	//Evitar de afastar do gol no Y
	if(expectedRobotPos.y > (LARGURA_CAMPO-TAMANHO_GOL)/2 + TAMANHO_GOL+deltaGolY){
		childFitness-= abs(50*(expectedRobotPos.y-(LARGURA_CAMPO-TAMANHO_GOL)/2-TAMANHO_GOL-deltaGolY));
	}else if (expectedRobotPos.y<(LARGURA_CAMPO-TAMANHO_GOL)/2-deltaGolY){
		childFitness-= abs(50*(expectedRobotPos.y-(LARGURA_CAMPO-TAMANHO_GOL)/2+deltaGolY));
	}

	//SITUACAO DE PERIGO DE GOL
	if(this->distanceFromGoal <30){
		//Antecipar a bola
		if(distanceGoalkeeper  < distanceCloserEnemie){
			
			// Sai do gol apenas se a bola estiver vindo para o gol
			double antecipFitness = 10000*(-cos(ballOrientation));
			
			//Quanto mais perto do goleiro melhor sair do gol
			antecipFitness /= pow(distanceGoalkeeper, 4);

			//Quanto mais perto for a posicao goleiro da posicao futura da bola melhor
			antecipFitness /= pow(EEGoalKeeper::distancePoints(rPos, futureBallPos), 4);

			childFitness+=antecipFitness;
		}

		if(distanceGoalkeeper<9){
			//Girar para chutar a bola
			childFitness *= pow(1+abs(child[0]-child[1]), 2);
		}
	}else{
		//Manter angulo reto quando seguindo o y da bola
		childFitness *= pow(sin(expectedOri), 2);

		//Nao girar parado na posicao
		childFitness /= pow(1+abs(child[0]-child[1]), 0.5);

	}
	//------------------------------------------------------//

	
	return childFitness;
}

void EEGoalKeeper::applyMutation(int* child){
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

void EEGoalKeeper::updateSigma(void){
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

double EEGoalKeeper::distancePoints(pt::Point A, pt::Point B){
	return sqrt(pow((A.x-B.x),2) + pow((A.y-B.y),2));
}