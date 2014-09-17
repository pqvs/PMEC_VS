#pragma once
#include "Tag.h"
//#include "Objeto.h"
#include "Robo.h"
#include "Bola.h"
#include "CLEyeMulticam.h"
#include "Point.h"
#include "define.h"
#include <opencv\cv.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv\highgui.h>
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <atomic>
//------ temporizador ---------
#include <windows.h>
#include <iostream>

#include <iostream>
#include <ctype.h>
using namespace cv;

class Visao{

public:
	Visao(Objeto **objetos);
	~Visao(void);
	void iniciar();	

private:
	bool modoCalibracao;
	void criarTags(Mat src);
	void criarTrackbars();
	void trackBarsConfigCamera();
	void eventoCalibrar();
	void inRangeThreshold(Mat src);
	void calculoHSV(Mat srcMat);
	void calibrarTamanhoImagem();
	void iniciarTracking();
	void filtroMedianaBackProject( int numeroVizinhos, Tag*tag);
	void setPosicoesObjetos();
	void desenharMarcacoes();
	void parearTags(vector<pt::Point> tagsSecundarias, vector<pt::Point> tagsPrimarias, vector<pt::Point> tagsPrimariasInimigas);
	int verifyCloserPoint(pt::Point point, vector<pt::Point> vecPoints, double range);
	double distanciaPontos(pt::Point A, pt::Point B);
	static void trackingTag(Tag* tag, Visao* instance);
	
	Mat frame;
	Mat threshold;
	Tag *tag[10];

	Point campoTop;
	Point campoBottom;
	Objeto **objetos;

	// Accurate Counter
	void StartCounter();
	double GetCounter();


	//Calibração Câmera

	// camera sensor parameters
	int AUTO_GAIN;				// [false; true]
	int GAIN;					// [0; 79]
	int AUTO_EXPOSURE;			// [false; true]
	int EXPOSURE;				// [0; 511]
	int AUTO_WHITEBALANCE;		// [false; true]
	int WHITEBALANCE_RED;		// [0; 255]
	int WHITEBALANCE_GREEN;		// [0; 255]
	int WHITEBALANCE_BLUE;		// [0; 255]

	// camera linear transform parameters (valid for MONO_PROCESSED; COLOR_PROCESSED modes)
	int HFLIP;					// [false; true]
	int VFLIP;					// [false; true]

	// camera non-linear transform parameters (valid for MONO_PROCESSED; COLOR_PROCESSED modes)
	int LENSCORRECTION1;		// [-500; 500]
	int LENSCORRECTION2;		// [-500; 500]
	int LENSCORRECTION3;		// [-500; 500]
	int LENSBRIGHTNESS; 		// [-500; 500]
};


