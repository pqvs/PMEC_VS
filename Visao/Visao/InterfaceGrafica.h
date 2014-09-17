#pragma once

#include <thread>
#include <atomic>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "Objeto.h"

using namespace std;
using namespace cv;
class InterfaceGrafica{
	public:
		InterfaceGrafica(Objeto** objetos);
		~InterfaceGrafica(void);
		void iniciar(atomic<bool>& quit);
	private:
		Objeto **objetos;
		vector<pt::Point> *lastBallPositions;
		double ballSpeed, ballOrientation;
		void desenharCampo(Mat frame);
		void desenharObjetos(Mat frame);
		void historicoPosicoesBola(Mat frame);
};

