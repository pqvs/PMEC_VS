#pragma once
#include <string>
#include <vector>
#include "Point.h"
#include <opencv\cv.h>

using namespace std;

class Tag{

public:
	Tag(string nome, int H_MIN, int H_MAX, int S_MIN, int S_MAX, int V_MIN, int V_MAX);
	~Tag(void);
	void setHSV(int H_MIN, int H_MAX, int S_MIN, int S_MAX, int V_MIN, int V_MAX, int A_MIN);

	cv::Mat threshold;
	cv::Rect trackWindow;
	vector<pt::Point> *posicoes;

	bool pronto;

	int * aH;
	int * aS;
	int * aV;

	string nome;
	int H_MIN;
	int H_MAX;
	int S_MIN;
	int S_MAX;
	int V_MIN;
	int V_MAX;
	int A_MIN;
};

