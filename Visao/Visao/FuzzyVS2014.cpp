// FuzzyVS2014.cpp : Defines the entry point for the console application.
//
#include "FuzzyVS2014.h"
/*
template <const uint8_t ROWS, const uint8_t COLUMNS>
const int** const FuzzyVS2014::castToPointer(const int (&a)[ROWS][COLUMNS]) {
	int **myArray = new int*[ROWS];
	for (uint8_t i = 0; i < ROWS; i++) {
		myArray[i] = (int*)a[i];
	}

	return (const int** const)myArray;
}
*/
vector< vector<int> > FuzzyVS2014::read_from_file(string file_name){
	//exemplo de implementação utilizando a lib FuzzyVS2014
	ifstream in(file_name);

	string line, field;

	vector< vector<int> > array;  // the 2D array
	vector<int> v;                // array of values for one line only

	while ( getline(in,line) )    // get next line in file
	{
		v.clear();
		stringstream ss(line);

		while (getline(ss,field,';'))  // break line into comma delimitted fields
		{
			v.push_back(atoi(field.c_str()));  // add each field to the 1D array
		}

		array.push_back(v);  // add the 1D array to the 2D array
	}
	return array;
}

vector<float> FuzzyVS2014::ControladorFuzzy(float angulo, float distancia){
	
	std::vector<float> v_output;
	std::vector<float> input;
	input.push_back(angulo);
	input.push_back(distancia);

	FuzzyVE ve = FuzzyVE("Rule_Base_VE.txt");
	float velocidadeEsq=ve.defuzzyfier(input);
	//cout<<"VEFuzzy: "<<velocidadeEsq;

	FuzzyVD vd = FuzzyVD("Rule_Base_VD.txt");
	float velocidadeDir=vd.defuzzyfier(input);
	//cout<<"  - VDFuzzy: "<<velocidadeDir<<endl;
	v_output.push_back(velocidadeEsq);
	v_output.push_back(velocidadeDir);
	return v_output;
}




FuzzyVS2014::FuzzyVS2014(void)
{
}


FuzzyVS2014::~FuzzyVS2014(void)
{
}
