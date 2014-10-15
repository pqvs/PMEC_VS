#pragma once

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include "FuzzyVE.h"
#include "FuzzyVD.h"
#include "TimerCounter.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


class FuzzyVS2014
{
public:

	FuzzyVS2014(void);
	~FuzzyVS2014(void);
	vector< vector<int> > read_from_file(string file_name);
	vector<float> ControladorFuzzy(float angulo, float distancia);
	//const int** const castToPointer(const int (&a)[ROWS][COLUMNS]);

};

