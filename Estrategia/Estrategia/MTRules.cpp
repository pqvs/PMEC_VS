#include "stdafx.h"
#include "MTRules.h"

MTRules::MTRules():baseRuleMatriz(nullptr){


}

MTRules::MTRules(std::string file_name)
{
	generate_baseRuleMatrix(file_name);
	this->printBaseRuleMatriz();
	//exemplo de implementação utilizando a lib FuzzyVS2014

}

MTRules::~MTRules()
{
}
void MTRules::generate_baseRuleMatrix(std::string file_name){
	//Read from file_name
	ifstream in(file_name);
	if(!in)
	{
		printf( "Erro na abertura do arquivo");
		getchar();
		exit(0);
	}
	string line, field;
	vector<vector<vector<int>>> array;
	vector< vector<int> > array2d;  // the 2D array
	vector<int> v;                // array of values for one line only
	int count=0;
	while ( getline(in,line) )    // get next line in file
	{
		v.clear();
		stringstream ss(line);
		if(line==""){
			array.push_back(std::move(array2d));
			array2d.clear();
			continue;
		}
		while (getline(ss,field,';'))  // break line into comma delimitted fields
		{
			v.push_back(atoi(field.c_str()));  // add each field to the 1D array
		}

		array2d.push_back(v);  // add the 1D array to the 2D array
	}
	v.clear();
	array.push_back(std::move(array2d));
	array2d.clear();
	//---------------------//
	this->O=array.size();
	this->D=array[this->O-1].size();
	this->ND=array[this->O-1][this->D-1].size();
	this->baseRuleMatriz=new int***;
	this->baseRuleMatriz[0]= new int**[this->O];
	for (int i = 0; i < this->O; i++) {
		this->baseRuleMatriz[0][i] = new int* [this->D];
		for (int j = 0; j < this->D; j++) {
			this->baseRuleMatriz[0][i][j] = new int[this->ND];
		}
	}


	for (int i = 0; i < O; i++) {
		for (int j = 0; j < D; j++) {
			for (int k = 0; k < ND; k++) {
				this->baseRuleMatriz[0][i][j][k]=array[i][j][k];
				//printf("%i : ",this->baseRuleMatriz[0][i][j][k]);
			}
			//printf("\n");
		}
		//printf("\n");
	}
}

float MTRules::fuzzy(float* inputs, std::vector<FuzzyFunction*> &input_variables,FuzzyFunction *output_variable){
	float input_O=inputs[0],input_D=inputs[1],input_ND=inputs[2];
	/*Trapez *varO=static_cast<Trapez*>((input_variables[0]).get()),*varD=static_cast<Trapez*>((input_variables[1]).get()),
	*varND=static_cast<Trapez*>((input_variables[2]).get());*/
	/*Trapez *varO=static_cast<Trapez*>((input_variables[0])),*varD=static_cast<Trapez*>((input_variables[1])),
	*varND=static_cast<Trapez*>((input_variables[2]));*/
	
	float *output_variable_array = (float*) calloc(output_variable->getnParticoes(), sizeof (float));
	for (int i = 0; i < output_variable->getnParticoes(); i++) {
		output_variable_array[i] = 0;
	}
	
	int****btemp=this->baseRuleMatriz;
	float temp = 1;
	float local = 0;
	for (int i = 0; i < O; i++) {
		for (int j = 0; j < D; j++) {
			for (int k = 0; k < ND; k++) {

				//local = varO->singleton(i, input_O);
				
				local = input_variables[0]->singleton(i, input_O);
				if (local < temp)
					temp = local;

				//local = varD->singleton(j, input_D);

				local = input_variables[1]->singleton(j, input_D);
				if (local < temp)
					temp = local;

				//local = varND->singleton(k, input_ND);
				
				local = input_variables[2]->singleton(k, input_ND);
				if (local < temp)
					temp = local;
				switch (btemp[0][i][j][k]) {
				case 2:
					//SA
					if (output_variable_array[0] < temp)
						output_variable_array[0] = temp;
					break;
				case 1:
					//A
					if (output_variable_array[1] < temp)
						output_variable_array[1] = temp;
					break;
				case 0:
					//N
					if (output_variable_array[2] < temp)
						output_variable_array[2] = temp;
					break;
				case -1:
					//D
					if (output_variable_array[3] < temp)
						output_variable_array[3] = temp;
					break;
				case -2:
					//SD
					if (output_variable_array[4] < temp)
						output_variable_array[4] = temp;
					break;
				default:
					break;
				}
				temp = 1;
			}
		}
	}
	for (int i = 0; i < output_variable->getnParticoes(); i++) {
		printf("%f : ", output_variable_array[i]);
	}
	printf("\n");
	return DeFuzzy::centroidMax2(output_variable, output_variable_array);
}

void MTRules::printBaseRuleMatriz() {
	for (int i = 0; i < this->O; i++) {
		for (int j = 0; j < this->ND; j++) {
			for (int k = 0; k < this->D; k++) {
				printf("%i : ", this->baseRuleMatriz[0][i][j][k]);
			}	
			printf("\n");
		}
		printf("\n");
	}
}