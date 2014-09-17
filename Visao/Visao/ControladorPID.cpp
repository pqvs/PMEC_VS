#include "stdafx.h"
#include "ControladorPID.h"


ControladorPID::ControladorPID(double Kp, double Ki, double Kd, Robo *robo)
{
	this->Kp  =Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	erroAnterior = 0;
	erroAcumulado = 0;
	this->robo = robo;
}

ControladorPID::ControladorPID(){}
ControladorPID::~ControladorPID(void)
{
}

/**
*	Calcula o erro e retorna a velocidade angular de cada roda
*	@param anguloAtual			Orientação atual em relação ao campo
*	@param anguloAlvo			Orientação do vetor velocidade em relação ao campo
*	@param velocidadeLinear		Módulo da velocidade do robo
*	@param pwm					Vetor de duas posicoes com o pwm: [0] -> dir, [1] -> esq
*/

void ControladorPID::PID2(double xb, double yb)
{
	double xi = robo->posicao.x;
    double yi = robo->posicao.y;
	double phi = robo->direcao;
	cout <<"Orientacao: "<< phi<<endl;
	if (xi != xAnterior || yb != yAnterior) {
		double phid = (atan2(abs(yb - yi), abs(xb - xi)));

        //quadrante
        if (xi < xb && yi < yb) {
            phid = 2 * PI - phid;

        } else if (xi > xb && yi < yb) {
            phid = PI + phid;

        } else if (xi > xb && yi > yb) {
            phid = PI - phid;
        }

        double e = (phid - phi);
        e = atan2(sin(e), cos(e));

        double e2;

        if (phi > PI)
            e2 = (phid - (phi - PI));
        else
            e2 = (phid - (phi + PI));

        e2 = atan2(sin(e2), cos(e2));
        bool re = false;
        // double r= ((double)(rand() % 10 + 1) -(double)(rand() % 10 + 1))/100;

        if (abs(e2) < abs(e)) {
            e = e2;
            re = true;
            if (!robo->direcao)
                this->erroAcumulado = 0;
            robo->direcao = true;
        } else {
            if (robo->direcao)
                this->erroAcumulado = 0;
            robo->direcao = false;
        }

        e = atan2(sin(e), cos(e));
        this->erroAcumulado += e;
        if (this->erroAcumulado > 40)
            this->erroAcumulado = 40;
        else if (this->erroAcumulado < -40)
            this->erroAcumulado = -40;

        //            cout<<integral<<endl;

        double w = 0;

        if (re)
			w = Kp * e + Ki * this->erroAcumulado + Kd * (e - this->erroAnterior);
		else
			w = Kp * e + Ki * this->erroAcumulado + Kd * (e - this->erroAnterior);

        double v = 70; ///log10(abs(w)+2);


		int vd = (int) ((2 * v + w * RAIO_ROBO) / (2 * RAIO_RODA_ROBO));
		int ve = (int) ((2 * v - w * RAIO_ROBO) / (2 * RAIO_RODA_ROBO));

        // stringstream sst;

		this->erroAnterior = e;
				// stringstream sst;

		if (re) {
			double esqTemp = ve;
			ve=-vd;
			vd=(int)-esqTemp;
		}

		robo->velocity_left= abs(ve)<PWM_MAX ? ve : PWM_MAX*ve/abs(ve);
		robo->velocity_right= abs(vd)<PWM_MAX ? vd : PWM_MAX*vd/abs(vd);
		cout << "Vd: "<<vd <<" Ve: "<< ve<< endl;
		xAnterior = xi;
		yAnterior = yi;
	}
	

}

void ControladorPID::execute(double anguloAtual, double anguloAlvo, double velocidadeLinear, int *pwm){
	anguloAlvo = atan2(sin(anguloAlvo), cos(anguloAlvo));
	double erro = anguloAlvo - anguloAtual;
	if(erro <0 && erro>=-PI/2)
		erro+=PI;
	else if(erro <=PI/2 && erro >= 0)
		erro-= PI;
	
	erroAcumulado = erroAcumulado + erro;

	double velocidadeAngular = Kp*erro + Ki*erroAcumulado + Kd*(erro-erroAnterior);

	erroAnterior = erro;	

	double Wd = (2*velocidadeLinear + RAIO_ROBO*velocidadeAngular)/(2*RAIO_RODA_ROBO);
	double We = (2*velocidadeLinear - RAIO_ROBO*velocidadeAngular)/(2*RAIO_RODA_ROBO);
	
	pwm[0] =(int)(We*PWM_MAX/VEL_MAX);
	pwm[1] =(int)(Wd*PWM_MAX/VEL_MAX);
	//cout <<"Dir1 "<<pwm[0] << " Esq1 " << pwm[1]<<endl;
}




void ControladorPID::PID(double xb, double yb) {
	
    double xi = robo->posicao.x;
    double yi =  robo->posicao.y;
	if (xi != xAnterior || yi != yAnterior) {
		double phi =  atan2(sin(robo->orientacao), cos(robo->orientacao));
		bool direcao = robo->direcao;

		double phid = (atan2(yb - yi, xb - xi));
		//cout <<"alvo: "<< phid*180/PI<<"  robo: "<< phi*180/PI<<endl;



		//double dx = (xi - x_anterior) / (dt * cos(phi/100));
		//double dy = (yi - y_anterior) / (dt * sin(phi/100));
		//double v = (dx > dy) ? dy : dx;

		double e = (phid - phi) ;
		e = atan2(sin(e), cos(e));
		//cout <<"Erro: "<< e*180/PI<<endl;
		//cout <<"Erro Anterior: "<< erroAnterior*180/PI<<endl;
		double e2;

		if (phi > PI)
			e2 = (phid - (phi - PI));
		else
			e2 = (phid - (phi + PI));

		e2 = atan2(sin(e2), cos(e2));
		//cout <<"Erro2: "<< e2*180/PI <<endl;
		bool re = false;

	  /* if (abs(e2) < abs(e)) {
			e = e2;
			re = true;
			if (!direcao){
				this->erroAcumulado = 0;
				this->erroAnterior=0;
			}
			direcao = true;
		} else {
			if (direcao){
				this->erroAcumulado = 0;
				this->erroAnterior = 0;
			}
			direcao = false;
		}*/

		this->erroAcumulado += e;
		if (this->erroAcumulado > 40)
			this->erroAcumulado = 40;
		else if (this->erroAcumulado < -40)
			this->erroAcumulado = -40;

		//            cout<<integral<<endl;

		double w = 0;
		double derivada = e - this->erroAnterior;
		//cout <<"Derivada: "<< 180*derivada/PI <<endl;
		if (re)
			w = Kp * e + Ki * this->erroAcumulado + Kd * (derivada);
		else
			w = Kp * e + Ki * this->erroAcumulado + Kd * (derivada);
			//cout <<"w: "<< w<<endl;
		double velocidadeLinear = 80; ///log10(abs(w)+2);


	//	//Calcula a velocidade obedecendo o limite
	//	double vLimite = (velocidadeLinear<VEL_MAX) ? velocidadeLinear : VEL_MAX;
	//	double wLimite =  (w < VEL_MAX/(2*RAIO_ROBO)) ? w : VEL_MAX/(2*RAIO_ROBO);

	//	//Calcula velocidade angular obedecendo os limites
	//	double Wd = (vLimite - RAIO_ROBO*wLimite)/(RAIO_RODA_ROBO);
	//	double We = (vLimite + RAIO_ROBO*wLimite)/(RAIO_RODA_ROBO);

	//	double wMax = (abs(Wd)<abs(We)) ? abs(We) : abs(Wd);

	//	if(wMax>W_RODA_MAX){
	//		Wd-= ((Wd)/abs(Wd))*(wMax -(W_RODA_MAX));
	//		We-= ((We)/abs(We))*(wMax -(W_RODA_MAX));
	//	}

	//	////Mantém o sinal e calcula a nova velocidade linear obedecendo os limites
	//	//velocidadeLinear = (velocidadeLinear/abs(velocidadeLinear))*RAIO_RODA_ROBO*(Wd+We)/2;
	//	//w = (w/abs(w))*RAIO_RODA_ROBO*(We-Wd)/(2*RAIO_ROBO);*/
		
	//	//Calcula velocidade angular das rodas mantendo os sinais
	double 	Wd = (velocidadeLinear - RAIO_ROBO*w)/(RAIO_RODA_ROBO);
	double We = (velocidadeLinear + RAIO_ROBO*w)/(RAIO_RODA_ROBO);

		//Wd *= PWM_MAX/W_RODA_MAX;
		//We *= PWM_MAX/W_RODA_MAX;

		/*double w_temp=We;
		We=-Wd;
		Wd=-w_temp;*/

		//Wd*=(Wd>We) ? 1.1 :  1;

		cout << "Wd: " << Wd << "\t We: " << We <<endl;


		this->erroAnterior = e;
				// stringstream sst;


		if (re) {
			double esqTemp = We;
			We=-Wd;
			Wd=-esqTemp;
		}


		robo->velocity_left		= (int)We;
		robo->velocity_right	= (int)Wd;

	}
	xAnterior = xi;
	yAnterior = yi;
}


void ControladorPID::resetConst(double Kp, double Ki, double Kd){
	this->Kp  =Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	this->erroAnterior = 0;
	this->erroAcumulado = 0;
}

void ControladorPID::setConst(double kp, double ki, double kd){
	this->Kp  =Kp;
	this->Ki = Ki;
	this->Kd = Kd;
}

void ControladorPID::pidTiaguera(pt::Point alvo, pt::Point atual, double orientacaoAtual){
	
	//////////////// PID ANGULO ///////////////////////////////
	double orientacaoAlvo = atan2(alvo.y - atual.y, alvo.x - atual.x);

	double erroW = atan2(sin(orientacaoAlvo-orientacaoAtual), cos(orientacaoAlvo-orientacaoAtual));

	
	if(erroAnteriorW==0)
		erroAnteriorW = erroW;

	erroAcumuladoW+=erroW;
	double W = KpW*erroW + KdW*(erroW - erroAnteriorW) + KiW*erroAcumuladoW;
	
	erroAnteriorW = erroW;
	/////////////////////////////////////////////////////////

	
	///////////// PID VELOCIDADE /////////////////////////////
	double  velocidadeLinear =0;
	//cout<<"erroW: "<<erroW*180/PI<<endl;
	//Se alvo nao foi atingido && a orientacao esta adequada (robo reto) -> velociaddeLinear!=0
	//
	if(abs(alvo.y-atual.y)>RAIO_ROBO && abs(erroW)<PI/6){
		double erroV = sqrt(pow(abs(alvo.y-atual.y),2) + pow(abs(alvo.x-atual.x),2));

		erroAcumuladoV+=erroV;

		velocidadeLinear= KpV*erroV + KdV*(erroV - erroAnteriorV) + KiV*erroAcumuladoV;

		erroAnteriorV = erroV;
		//cout<<"vel linear"<<endl;
	}
	////////////////////////////////////////////////////////



	double Wd = (velocidadeLinear - RAIO_ROBO*W)/(RAIO_RODA_ROBO);
	double We = (velocidadeLinear + RAIO_ROBO*W)/(RAIO_RODA_ROBO);

	robo->velocity_left =  (int)(We*PWM_MAX/VEL_MAX);
	robo->velocity_right =  (int)(Wd*PWM_MAX/VEL_MAX);
}