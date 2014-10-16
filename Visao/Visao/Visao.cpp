#include "Visao.h"

double PCFreq = 0.0;
__int64 CounterStart = 0;
// -----------------------------



using namespace std;

using namespace cv;
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
int A_MIN;

int indiceCalibracao = 0;

int *aH0,*aS0,*aV0;
int *aH1,*aS1,*aV1;
int *aH2,*aS2,*aV2;
int *aH3,*aS3,*aV3;
int *aH4,*aS4,*aV4;
int *aH5,*aS5,*aV5;

uchar *data0;
uchar *data1;
uchar *data2;
uchar *data3;
uchar *data4;
uchar *data5;

condition_variable loopCondition;
condition_variable trackingConditionVar;
bool break_thread = false;
atomic<int> finishedThreads (0) ;

Visao::Visao(Objeto **objetos){
	modoCalibracao = true;
	campoTop = Point(0, 0);
    campoBottom = Point(FRAME_WIDTH, FRAME_HEIGHT);
	this->objetos = objetos;
	try{
		ifstream txtFile;
		string linha;
		txtFile.open("configcampo.txt");
		getline(txtFile, linha);campoTop.x = atoi(linha.c_str());
		getline(txtFile, linha);campoTop.y = atoi(linha.c_str());
		getline(txtFile, linha);campoBottom.x = atoi(linha.c_str());
		getline(txtFile, linha);campoBottom.y = atoi(linha.c_str());
	}catch(Exception e){
		cout<<"Falha na leitura configcampo.txt :"<<e.err<<endl;
	}

}


Visao::~Visao(void){
}

void Visao::iniciar(){

	Mat HSV;

	criarTags(frame);
	criarTrackbars();
	
	aH0 = this->tag[0]->aH; aS0 = this->tag[0]->aS; aV0 = this->tag[0]->aV;
	aH1 = this->tag[1]->aH; aS1 = this->tag[1]->aS; aV1 = this->tag[1]->aV;
	aH2 = this->tag[2]->aH; aS2 = this->tag[2]->aS; aV2 = this->tag[2]->aV;
	aH3 = this->tag[3]->aH; aS3 = this->tag[3]->aS; aV3 = this->tag[3]->aV;
	aH4 = this->tag[4]->aH; aS4 = this->tag[4]->aS; aV4 = this->tag[4]->aV;
	aH5 = this->tag[5]->aH; aS5 = this->tag[5]->aS; aV5 = this->tag[5]->aV;

	data0 = tag[0]->threshold.data;
	data1 = tag[1]->threshold.data;
	data2 = tag[2]->threshold.data;
	data3 = tag[3]->threshold.data;
	data4 = tag[4]->threshold.data;
	data5 = tag[5]->threshold.data;
	CLEyeCameraInstance cam;
	cam = CLEyeCreateCamera(CLEyeGetCameraUUID(0),CLEYE_COLOR_PROCESSED ,CLEYE_VGA,75);
	cout << "Here" << endl; 
	
	CLEyeCameraStart(cam);
	IplImage *pCapImage;
	pCapImage = cvCreateImage(cvSize(FRAME_WIDTH, FRAME_HEIGHT), IPL_DEPTH_8U, 4);
	PBYTE pCapBuffer = NULL; 
	cvGetImageRawData(pCapImage, &pCapBuffer);
	frame = Mat(pCapImage);
	trackBarsConfigCamera();

	thread threads[6];
	for (int i=0; i<6; i++)
		threads[i] = thread(trackingTag,tag[i], this);

	while(1){
		CLEyeCameraGetFrame(cam,pCapBuffer,0);
		imshow("Video", frame);
		
		
		if(AUTO_GAIN){
			CLEyeSetCameraParameter(cam, CLEYE_AUTO_GAIN, AUTO_GAIN);
		}else{
			CLEyeSetCameraParameter(cam, CLEYE_AUTO_GAIN, AUTO_GAIN);
			CLEyeSetCameraParameter(cam, CLEYE_GAIN, GAIN);
		}
		if(AUTO_EXPOSURE){
			CLEyeSetCameraParameter(cam, CLEYE_AUTO_EXPOSURE, AUTO_EXPOSURE);
		}else{
			CLEyeSetCameraParameter(cam, CLEYE_EXPOSURE, EXPOSURE);
			CLEyeSetCameraParameter(cam, CLEYE_AUTO_EXPOSURE, AUTO_GAIN);
		}

		if(AUTO_WHITEBALANCE){
			CLEyeSetCameraParameter(cam, CLEYE_AUTO_WHITEBALANCE, AUTO_WHITEBALANCE);
		}else{
			CLEyeSetCameraParameter(cam, CLEYE_WHITEBALANCE_RED, WHITEBALANCE_RED);
			CLEyeSetCameraParameter(cam, CLEYE_WHITEBALANCE_GREEN, WHITEBALANCE_GREEN);
			CLEyeSetCameraParameter(cam, CLEYE_WHITEBALANCE_BLUE, WHITEBALANCE_BLUE);
			CLEyeSetCameraParameter(cam, CLEYE_AUTO_WHITEBALANCE, AUTO_WHITEBALANCE);
		}

		CLEyeSetCameraParameter(cam, CLEYE_HFLIP, HFLIP);
		CLEyeSetCameraParameter(cam, CLEYE_VFLIP, VFLIP);

		CLEyeSetCameraParameter(cam, CLEYE_LENSCORRECTION1, LENSCORRECTION1-500);
		CLEyeSetCameraParameter(cam, CLEYE_LENSCORRECTION2, LENSCORRECTION2-500);
		CLEyeSetCameraParameter(cam, CLEYE_LENSCORRECTION3, LENSCORRECTION3-500);
		CLEyeSetCameraParameter(cam, CLEYE_LENSBRIGHTNESS, LENSBRIGHTNESS-500);


		char c = waitKey(1);
		if(c == 'z' || c == 'Z')
			break;
	}
	while(1){
			
		CLEyeCameraGetFrame(cam,pCapBuffer);
		// Determina o HSV da imagem.]
		
		double t1 = getTickCount();StartCounter();
		medianBlur(frame,frame,3);
		cvtColor(frame,HSV,COLOR_BGR2HSV);
		// Cálculo do threshold da imagem
		inRangeThreshold(HSV);

		
		// Tracking - busca a posição de uma cor na imagem.
		iniciarTracking();
		
		setPosicoesObjetos();
		// Verifica as teclas apertadas e caso especificado, inicia o modo de calibração.
		eventoCalibrar();
		//
		//cout<<"Tempo: "<<1000*(getTickCount()-t1)/getTickFrequency()<<" , "<<GetCounter()<<endl;
		
	}

	//Para as threads de tracking
	break_thread = true;
	trackingConditionVar.notify_all();

	for (auto& th : threads) th.join();

}

void Visao::criarTags(Mat src){
	// Tag principal (Azul ou amarelo)
	tag[0] = new Tag("principal", 0,255,0,255,0,255);

	// Tags secundárias
	tag[1] = new Tag("tag 0", 0,255,0,255,0,255);
	tag[2] = new Tag("tag 1", 0,255,0,255,0,255);
	tag[3] = new Tag("tag 2", 0,255,0,255,0,255);

	// Bola
	tag[4] = new Tag("bola", 0,255,0,255,0,255);

	// Tag adversária
	tag[5] = new Tag("adversario", 0,255,0,255,0,255);

	ifstream txtFile;
    string linha ;
	int numlinha = 0;
    txtFile.open("config.txt");
	getline(txtFile, linha);
	string valores[7];
    while(!txtFile.eof() && numlinha < 6){
		char characters[30];
		strcpy(characters,linha.c_str());
		char* chars = strtok(characters, ";");
		int i = 0;
		if(chars)
			valores[i] = chars;
		while(chars){
			i++;
			chars = strtok(NULL, ";");
			if(chars)
				valores[i] = chars;
		}
		if(atoi(valores[1].c_str()) <= 10 || atoi(valores[3].c_str()) <= 10 || atoi(valores[5].c_str()) <= 10){
			numlinha++;
			continue;
		}
		tag[numlinha]->setHSV(atoi(valores[0].c_str()),atoi(valores[1].c_str()),atoi(valores[2].c_str()),atoi(valores[3].c_str()),atoi(valores[4].c_str()),atoi(valores[5].c_str()), atoi(valores[6].c_str()));

        getline(txtFile, linha);
		numlinha++;
    }
    txtFile.close();
}

void Visao::criarTrackbars(){
	//atualiza os valores de HSV min e max.
	H_MIN = tag[indiceCalibracao]->H_MIN, H_MAX = tag[indiceCalibracao]->H_MAX;
	S_MIN = tag[indiceCalibracao]->S_MIN, S_MAX = tag[indiceCalibracao]->S_MAX;
	V_MIN = tag[indiceCalibracao]->V_MIN, V_MAX = tag[indiceCalibracao]->V_MAX;
	A_MIN = tag[indiceCalibracao]->A_MIN;
	// nome da janela
	string trackbarWindow = "Trackbars";
	//cria window para trackbars
	namedWindow(trackbarWindow,0);
	resizeWindow(trackbarWindow, 350, 400);

	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	sprintf( TrackbarName, "A_MIN", A_MIN);

	// Cria a trackbar e insere na window.
	// os 3 parâmetros são: o endereco da variavel que será mudado quando a barra é movida, o máximo valor do trackbar
	// e a função chamada sempre que o trackbar é movido.
	createTrackbar( "H_MIN", trackbarWindow, &H_MIN, 256);
	createTrackbar( "H_MAX", trackbarWindow, &H_MAX, 256);
	createTrackbar( "S_MIN", trackbarWindow, &S_MIN, 256);
	createTrackbar( "S_MAX", trackbarWindow, &S_MAX, 256);
	createTrackbar( "V_MIN", trackbarWindow, &V_MIN, 256);
	createTrackbar( "V_MAX", trackbarWindow, &V_MAX, 256);
	createTrackbar( "A_MIN", trackbarWindow, &A_MIN, 300);

	// TrackBars para corte do capo."
	namedWindow("Campo", 1); // Janela para sliders de ajuste de ajuste do campo
	resizeWindow("Campo", 320, 280);
    createTrackbar("Left", "Campo", &campoTop.x, FRAME_WIDTH, 0);
	createTrackbar("Top", "Campo", &campoTop.y, FRAME_HEIGHT, 0);
	createTrackbar("Right", "Campo", &campoBottom.x, FRAME_WIDTH, 0);
	createTrackbar("Bottom", "Campo", &campoBottom.y, FRAME_HEIGHT, 0);
}

void Visao::trackBarsConfigCamera(){
	// nome da janela
	string trackbarWindow = "Trackbars Câmera";
	//cria window para trackbars
	namedWindow(trackbarWindow,0);
	resizeWindow(trackbarWindow, 350, 400);	
		
	ifstream txtFile;
    string linha ;
    txtFile.open("cfg_cam.txt");
	getline(txtFile, linha);	
	AUTO_GAIN			= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	GAIN				= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	AUTO_EXPOSURE		= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	EXPOSURE			= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	AUTO_WHITEBALANCE	= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	WHITEBALANCE_RED	= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	WHITEBALANCE_GREEN	= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	WHITEBALANCE_BLUE	= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	HFLIP				= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	VFLIP				= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	LENSCORRECTION1		= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	LENSCORRECTION2		= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	LENSCORRECTION3		= (int)atoi(linha.c_str());
	getline(txtFile, linha);	
	LENSBRIGHTNESS		= (int)atoi(linha.c_str());
    txtFile.close();

	char TrackbarName[50];
	sprintf( TrackbarName, "auto gain", AUTO_GAIN);
	sprintf( TrackbarName, "gain", GAIN);
	sprintf( TrackbarName, "auto exposure", AUTO_EXPOSURE);
	sprintf( TrackbarName, "exposure", EXPOSURE);
	sprintf( TrackbarName, "auto balance", AUTO_WHITEBALANCE);
	sprintf( TrackbarName, "red", WHITEBALANCE_RED);
	sprintf( TrackbarName, "green", WHITEBALANCE_GREEN);
	sprintf( TrackbarName, "blue", WHITEBALANCE_BLUE);
	sprintf( TrackbarName, "hflip", HFLIP);
	sprintf( TrackbarName, "vflip", VFLIP);
	sprintf( TrackbarName, "lens corr.1", LENSCORRECTION1);
	sprintf( TrackbarName, "lens corr.2", LENSCORRECTION2);
	sprintf( TrackbarName, "lens corr.3", LENSCORRECTION3);
	sprintf( TrackbarName, "lens brightness", LENSBRIGHTNESS);


	// Cria a trackbar e insere na window.
	// os 3 parâmetros são: o endereco da variavel que será mudado quando a barra é movida, o máximo valor do trackbar
	// e a função chamada sempre que o trackbar é movido.
	createTrackbar( "auto gain", trackbarWindow, &AUTO_GAIN, 1);
	createTrackbar( "gain", trackbarWindow, &GAIN, 79);
	createTrackbar( "auto exposure", trackbarWindow, &AUTO_EXPOSURE, 1);
	createTrackbar( "exposure", trackbarWindow, &EXPOSURE, 511);
	createTrackbar( "auto balance", trackbarWindow, &AUTO_WHITEBALANCE, 1);
	createTrackbar( "red", trackbarWindow, &WHITEBALANCE_RED, 255);
	createTrackbar( "green", trackbarWindow, &WHITEBALANCE_GREEN, 255);
	createTrackbar( "blue", trackbarWindow, &WHITEBALANCE_BLUE, 255);
	createTrackbar( "hflip", trackbarWindow, &HFLIP, 1);
	createTrackbar( "vflip", trackbarWindow, &VFLIP, 1);
	createTrackbar( "lens corr.1", trackbarWindow, &LENSCORRECTION1, 1000);
	createTrackbar( "lens corr.2", trackbarWindow, &LENSCORRECTION2, 1000);
	createTrackbar( "lens corr.3", trackbarWindow, &LENSCORRECTION3, 1000);
	createTrackbar( "lens brightness", trackbarWindow, &LENSBRIGHTNESS, 1000);



}


void Visao::inRangeThreshold(Mat src){

	tag[0]->threshold = Scalar::all(0);
	tag[1]->threshold = Scalar::all(0);
	tag[2]->threshold = Scalar::all(0);
	tag[3]->threshold = Scalar::all(0);
	tag[4]->threshold = Scalar::all(0);
	tag[5]->threshold = Scalar::all(0);

	uchar *srcData = src.data;

	float R, G, B, min, H, S, V, delta;
	int linhaInicial = campoTop.x, linhaFinal = campoBottom.x, colunaInicial=campoTop.y, colunaFinal = campoBottom.y;

	
	for(int i = colunaInicial; i < colunaFinal; i++){
		for(int j = linhaInicial; j < linhaFinal; j++){
			
			int ec = FRAME_WIDTH*i + j;
			int e3c = ec*3;

			int H = srcData[e3c];
			int S = srcData[e3c + 1];
			int V = srcData[e3c + 2];


			if(aH0[H] && aS0[S] && aV0[V]){
				data0[ec] = 255;
			}

			if(aH1[H] && aS1[S] && aV1[V]){
				data1[ec] = 255;
			}

			if(aH2[H] && aS2[S] && aV2[V]){
				data2[ec] = 255;
			}

			if(aH3[H] && aS3[S] && aV3[V]){
				data3[ec] = 255;
			}

			if(aH4[H] && aS4[S] && aV4[V]){
				data4[ec] = 255;
			}

			if(aH5[H] && aS5[S] && aV5[V]){
				data5[ec] = 255;
			}
			ec++;
		}
	}
	//Mat dst;
	//inRange(src,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),dst);
	//imshow("opencv",dst);
}

void Visao::trackingTag(Tag* tag, Visao* instance){
	mutex mtx;
	unique_lock<std::mutex> lck(mtx);
	while (!break_thread){ 
		
		//Espera para realizar o tracking e passar o filtro
		trackingConditionVar.wait(lck);

		Mat threshold = tag->threshold;
		vector<pt::Point> *posicoes = tag->posicoes;
		instance->filtroMedianaBackProject(5, tag);
		Mat temp;
		threshold.copyTo(temp);
		int x, y, j = 0;
		//dois vetores usados como saida da função findContours
		vector< vector<Point> > contours;
		vector<Vec4i> hierarchy;
		//encontra os contornos da imagem filtrada
		cv::findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
		if (hierarchy.size() > 0) {
			int index = 0;
			posicoes->clear();
			while(j < 3 && index >= 0){
				cv::Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				//Se a área do objeto for muito pequena então provavelmente deve ser apenas ruído.
				if(area >= tag->A_MIN){
					x = moment.m10/area;
					y = moment.m01/area;
					posicoes->push_back(pt::Point(x,y));
					j++;
				}
				index = hierarchy[index][0];
			}
		}
		finishedThreads.fetch_add(1);
	}
}
void Visao::filtroMedianaBackProject(int numeroVizinhos, Tag*tag){

	int linhaInicial = campoTop.x, linhaFinal = campoBottom.x, colunaInicial=campoTop.y, colunaFinal = campoBottom.y;
	Mat srcMat = tag->threshold;
	
	//int* window = new int[numeroVizinhos*numeroVizinhos];
	uchar *data = srcMat.data;
	int delta = 15;
	bool taDentro;
	float media;
	for(int j = colunaInicial; j < colunaFinal; j++){
		size_t val = FRAME_WIDTH*j;
		for(int i = linhaInicial; i < linhaFinal; i++){
			taDentro = false;
			media=0;
			for(int idxPosicao=0; idxPosicao<tag->posicoes->size();  idxPosicao++){
				if(j<tag->posicoes->at(idxPosicao).x+delta && j>tag->posicoes->at(idxPosicao).x-delta && i<tag->posicoes->at(idxPosicao).y+delta && i>tag->posicoes->at(idxPosicao).y-delta){
					taDentro = true;
					break;
				}
			}

			if(data[val+i]>0 || taDentro){
				for(int idxWLinha = 0; idxWLinha<numeroVizinhos; idxWLinha++){
					for(int idxWColuna = 0; idxWColuna<numeroVizinhos; idxWColuna++){
						try{
							media += srcMat.at<uchar>(j - (numeroVizinhos-1)/2 + idxWColuna, i - (numeroVizinhos-1)/2 + idxWLinha);
						}catch(Exception e){
							cout<<e.code<<","<<e.err<<","<<e.file<<endl;
							waitKey(10000);
						}
					}
				}
 
				if((media/(numeroVizinhos*numeroVizinhos))>127)
					srcMat.at<uchar>(j, i) = 255;
				else
					srcMat.at<uchar>(j, i) = 0;

            }
		}
	}

}



void Visao::iniciarTracking(){

	finishedThreads.store(0);
	tag[0]->pronto=false;
	tag[1]->pronto=false;
	tag[2]->pronto=false;
	tag[3]->pronto=false;
	tag[4]->pronto=false;
	tag[5]->pronto=false;

	trackingConditionVar.notify_all();

	while(finishedThreads.load()<6);	

}

void Visao::setPosicoesObjetos(){
	float scaleX = (COMPRIMENTO_CAMPO+2*BORDA_CAMPO)/(campoBottom.x - campoTop.x);
	float scaleY = LARGURA_CAMPO/(campoBottom.y - campoTop.y);
	pt::Point cTag;
	int tagPar;
	bool pareado = false;
	int size = tag[0]->posicoes->size();
	if(size < 1)
		return;
	for(int i = 1; i < 4; i++){
		if(tag[i]->posicoes->size() == 1){
			pareado = true;
			tagPar = 0;
			cTag = tag[i]->posicoes->at(0);
			double menorDistancia = distanciaPontos(tag[0]->posicoes->at(tagPar), cTag);
			for(int j = 1; j < size; j++){
				double distAux = distanciaPontos(tag[0]->posicoes->at(j), cTag);
				if(menorDistancia > distAux){
					tagPar = j;
					menorDistancia = distAux;
				}
			}
		}else if(tag[i]->posicoes->size() > 1){

			float maxPairRange = 20;

			for(int iTag = 0; iTag<tag[i]->posicoes->size(); iTag++){

				vector<Point> closerTags, enemyPrimaryCloserTags, enemySecundaryCloserTags;
				int closerTag = verifyCloserPoint(tag[i]->posicoes->at(iTag), *tag[0]->posicoes, maxPairRange);

				if(closerTag >=0){
					int closerEnemyTag = verifyCloserPoint(tag[i]->posicoes->at(iTag), *tag[5]->posicoes, maxPairRange);

					if(closerEnemyTag >= 0){

						float distanciaPrimaria = distanciaPontos(tag[i]->posicoes->at(iTag),tag[0]->posicoes->at(closerTag));
						float distanciaInimiga = distanciaPontos(tag[i]->posicoes->at(iTag),tag[5]->posicoes->at(closerEnemyTag));
				
			
						if(distanciaPrimaria<distanciaInimiga){
							//Essa tag e a secundaria nossa
							cTag=tag[i]->posicoes->at(iTag);
							tagPar=closerTag;
							pareado = true;
						}else{
							//Essa tag e a secundaria do inimigo
						}
					}else{
						//So tem uma primaria perto, a nossa
						cTag=tag[i]->posicoes->at(iTag);
						tagPar=closerTag;
						pareado = true;
					}
				}else{
					//Nao tem primaria perto
				}
			}
		}


		if(pareado){
			int roboX = (int) (cTag.x + tag[0]->posicoes->at(tagPar).x)/2;
			int roboY = (int) (cTag.y + tag[0]->posicoes->at(tagPar).y)/2;

			int dx = FRAME_WIDTH/2 - roboX;
			int dy = FRAME_HEIGHT/2 - roboY;
			
			roboX += (int)(dx - dx*SCALE_CAM);
			roboY += (int)(dy - dy*SCALE_CAM);

			

			this->objetos[i-1]->posicao.setp((roboX-campoTop.x)*scaleX, (roboY-campoTop.y)*scaleY);
			// atan(Y/X)
			double orientacao = atan2(cTag.y - tag[0]->posicoes->at(tagPar).y, cTag.x - tag[0]->posicoes->at(tagPar).x);
			orientacao += PI/4;

			if(orientacao > 2*PI)
				orientacao -= 2*PI;
			if(orientacao/ this->objetos[i-1]->orientacao<0&&abs(orientacao)>PI/2)
				this->objetos[i-1]->orientacao = (orientacao + this->objetos[i-1]->orientacao)/2 +PI;
			else
				this->objetos[i-1]->orientacao = (orientacao + this->objetos[i-1]->orientacao)/2;// average
		}
	}
	if(tag[4]->posicoes->size() > 0)
		this->objetos[3]->posicao.setp((tag[4]->posicoes->at(0).x - campoTop.x)*scaleX, (tag[4]->posicoes->at(0).y - campoTop.y)*scaleY);
	
	for(int i = 0, j = 4; i < tag[5]->posicoes->size(); i++, j++){
		this->objetos[j]->posicao.setp((tag[5]->posicoes->at(i).x - campoTop.x)*scaleX,(tag[5]->posicoes->at(i).y-campoTop.y)*scaleY);
	}
}


	
/*
*	Retorna o indice no vetor vecPoints do ponto mais proximo
*/
int Visao::verifyCloserPoint(pt::Point point, vector<pt::Point> vecPoints, float range){
	
		if(range<0)
			range = FLT_MAX;
		int retorno = -1;
		//number of points to verify
		int numberPoints = vecPoints.size();

		//x and y position from the primary point
		int xPrimary = point.x, yPrimary = point.y;
		
		//distance from the closer secundary point
		float closerDitancePoint = FLT_MAX;
		
		//Get the closer secundary point
		for(int j=0; j<numberPoints; j++){
			pt::Point secundaryPoint = vecPoints.at(j);
			//Distance from the primary point to the secundary point
			float distance = distanciaPontos(secundaryPoint, point);
	
			//Verify if is on the range
			if(distance<range){
				//Verify who is the closer point
				if(distance<closerDitancePoint){
					closerDitancePoint = distance;
					retorno = j;
				}
			}
		}
		return retorno;
}

double Visao::distanciaPontos(pt::Point A, pt::Point B){
	return sqrt(pow((A.x-B.x),2) + pow((A.y-B.y),2));
}

void Visao::calculoHSV(Mat srcMat){
	//Pula area sem interessse
	float R, G, B, min, H, S, V, delta;
	int linhaInicial = campoTop.x, linhaFinal = campoBottom.x, colunaInicial=campoTop.y, colunaFinal = campoBottom.y;

	for(int i = colunaInicial; i < colunaFinal; i++){
		for(int j = linhaInicial; j < linhaFinal; j++){
			size_t val = FRAME_WIDTH*3*i + 3*j;

			R = srcMat.data[val + 2]/255.0f; 
			G = srcMat.data[val + 1]/255.0f;
			B = srcMat.data[val]/255.0f;
   
			if(R > G){
				//V e o max
				V = (R > B) ? R : B;
				min = (G < B) ? G : B;
			}else{
				//V e o max
				V = (G > B) ? G : B;
				min = (R < B) ? R : B;
			}

			delta = V - min; 

			if(delta == 0){
				//Se delta=0 H e S sao 0
				H = 0.0f;
				S = 0.0f;
			}else{
				if(V == R){
					H = 60.0f*((G-B)/delta) ;
					H+= ((H<0) ? 360.0f : 0.0f);
				}else if(V == G)
					H = 60.0f*(((B-R)/delta)+2.0f);
				else
					H = 60.0f*(((R-G)/delta)+4.0f);

				//Nao precisa verificar max pois se max=0 entao delta=0, logo entrou na condicao anterior
				S = delta/V;
			}

			srcMat.data[val] = (int)(H*255.0f/360.0f);
			srcMat.data[val+1] = (int)(S*255.0f);
			srcMat.data[val+2] = (int)(V*255.0f);
			}
	}
}

void Visao::eventoCalibrar(){
		SHORT tabKeyState = GetAsyncKeyState(VK_TAB);
		// Test high bit - if set, key was down when GetAsyncKeyState was called.
		if(tabKeyState){
			if(modoCalibracao){
				cout<<"DESATIVADO ! ! !"<<endl;
				destroyAllWindows();
			}else{
				cout<<"ATIVADO ! ! !"<<endl;
				criarTrackbars();
			}
			modoCalibracao = !modoCalibracao;
			while(GetAsyncKeyState(VK_TAB));
		}


		if(modoCalibracao){
			calibrarTamanhoImagem();
			desenharMarcacoes();
			imshow("Threshold",tag[indiceCalibracao]->threshold);
			imshow("Video",frame);
			char c = waitKey(1);
			bool mudou = false;
			if(c == '1'){
				if(indiceCalibracao >0){
					mudou = true;
					indiceCalibracao--;
				}
			}else if(c == '2'){
				if(indiceCalibracao <5){
					mudou = true;
					indiceCalibracao++;
				}
			}else if(c == 's'){
				ofstream txtFile;
				txtFile.open("config.txt");
				for(int i = 0; i < 6; i++){
					txtFile<<tag[i]->H_MIN<<";"<<tag[i]->H_MAX<<";"<<tag[i]->S_MIN<<";"<<tag[i]->S_MAX<<";"<<tag[i]->V_MIN<<";"<<tag[i]->V_MAX<<";"<<tag[i]->A_MIN<<"\n";
				}
				txtFile.close();

				txtFile.open("cfg_cam.txt");
				txtFile << AUTO_GAIN << endl;
				txtFile << GAIN << endl;
				txtFile << AUTO_EXPOSURE << endl;
				txtFile << EXPOSURE << endl;
				txtFile << AUTO_WHITEBALANCE << endl;
				txtFile << WHITEBALANCE_RED << endl;
				txtFile << WHITEBALANCE_GREEN << endl;
				txtFile << WHITEBALANCE_BLUE << endl;
				txtFile << HFLIP << endl;
				txtFile << VFLIP << endl;
				txtFile << LENSCORRECTION1 << endl;
				txtFile << LENSCORRECTION2 << endl;
				txtFile << LENSCORRECTION3 << endl;
				txtFile << LENSBRIGHTNESS << endl;
				txtFile.close();

				txtFile.open("configcampo.txt");
				txtFile<<campoTop.x<<endl;
				txtFile<<campoTop.y<<endl;
				txtFile<<campoBottom.x<<endl;
				txtFile<<campoBottom.y<<endl;
				txtFile.close();
			}

			if(mudou){
				criarTrackbars();
			}

			tag[indiceCalibracao]->setHSV(H_MIN,H_MAX,S_MIN,S_MAX,V_MIN,V_MAX, A_MIN);
			
			system("cls");
			cout<< "Calibrando indice: "<<indiceCalibracao<<endl;
			cout<< "Modo de Calibração: "<<modoCalibracao<<endl;
			cout<< "Tag: "<<tag[indiceCalibracao]->nome<<endl;
		}
}

void Visao::desenharMarcacoes(){
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < tag[i]->posicoes->size(); j++){			
			cv::Point point(tag[i]->posicoes->at(j).x,tag[i]->posicoes->at(j).y);
			circle(frame,point, 3, Scalar(0,0,255));
		}

	}



	float scaleX = (campoBottom.x - campoTop.x)/(COMPRIMENTO_CAMPO+2*BORDA_CAMPO);
	float scaleY = (campoBottom.y - campoTop.y)/LARGURA_CAMPO;


	//Desenha os jogadores
	for(int i=0; i<3; i++){
		Point posicao(this->objetos[i]->posicao.x*scaleX + campoTop.x, this->objetos[i]->posicao.y*scaleY + campoTop.y);
		float deltaX = cos(objetos[i]->orientacao)*14;
		float deltaY = sin(objetos[i]->orientacao)*14;
		line(frame, posicao, Point(posicao.x+deltaX, posicao.y+deltaY), Scalar(255, 255, 255), 2, 8, 0);
		circle(frame, posicao, 14, Scalar(255, 255, 255), 1, 8, 0);
	}



}

void Visao::calibrarTamanhoImagem(){
	circle(frame, campoTop, 3, Scalar(255, 255, 255), 1, 8, 0);
	circle(frame, campoBottom, 3, Scalar(255, 255, 255), 1, 8, 0);
	line(frame, Point(0, campoTop.y), Point(FRAME_WIDTH, campoTop.y), Scalar(255, 255, 255), 1, 1, 0);
	line(frame, Point(0, campoBottom.y), Point(FRAME_WIDTH, campoBottom.y), Scalar(255, 255, 255), 1, 1, 0);
	line(frame, Point(campoTop.x, 0), Point(campoTop.x, FRAME_HEIGHT), Scalar(255, 255, 255), 1, 1, 0);
	line(frame, Point(campoBottom.x, 0), Point(campoBottom.x, FRAME_HEIGHT), Scalar(255, 255, 255), 1, 1, 0);
}



void Visao::StartCounter(){
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
	cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double Visao::GetCounter(){
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}