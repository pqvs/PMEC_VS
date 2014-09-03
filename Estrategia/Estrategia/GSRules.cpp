#include "stdafx.h"
#include "GSRules.h"


GSRules::GSRules(std::string file_name)
{
	generate_baseRuleMatrix(file_name);
	this->printBaseRuleMatriz();

}


GSRules::~GSRules()
{
}

GSRules::GSRules():baseRuleMatriz(nullptr){


}

void GSRules::generate_baseRuleMatrix(std::string file_name){
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
			if (field.find("N") != string::npos)
				v.push_back(999);
			else
				v.push_back(atoi(field.c_str()));  // add each field to the 1D array
		//printf("%s : ",field.c_str());
		}

		array2d.push_back(v);  // add the 1D array to the 2D array
	}
	v.clear();
	array.push_back(std::move(array2d));
	array2d.clear();
	//---------------------//
	this->SW=array.size();
	this->Bx=array[this->SW-1].size();
	this->By=array[this->SW-1][this->Bx-1].size();
	this->baseRuleMatriz=new int***;
	this->baseRuleMatriz[0]= new int**[this->SW];
	for (int i = 0; i < this->SW; i++) {
		this->baseRuleMatriz[0][i] = new int* [this->Bx];
		for (int j = 0; j < this->Bx; j++) {
			this->baseRuleMatriz[0][i][j] = new int[this->By];
		}
	}


	for (int i = 0; i < this->SW; i++) {
		for (int j = 0; j < this->Bx; j++) {
			for (int k = 0; k < this->By; k++) {
				this->baseRuleMatriz[0][i][j][k]=array[i][j][k];
				//printf("%i : ",this->baseRuleMatriz[0][i][j][k]);
			}
			//printf("\n");
		}
		//printf("\n");
	}
}

float GSRules::fuzzy(float* inputs, std::vector<FuzzyFunction*> &input_variables,FuzzyFunction *output_variable){
	float input_SW=inputs[0],input_Bx=inputs[1],input_By=inputs[2];
	/*Trapez *varO=static_cast<Trapez*>((input_variables[0]).get()),*varD=static_cast<Trapez*>((input_variables[1]).get()),
	*varND=static_cast<Trapez*>((input_variables[2]).get());*/
	float *output_variable_array = (float*) calloc(output_variable->getnParticoes(), sizeof (float));
	for (int i = 0; i < output_variable->getnParticoes(); i++) {
		output_variable_array[i] = 0;
	}
	float temp = 1;
	float local = 0;
	for (int i = 0; i < this->SW; i++) {
		for (int j = 0; j < this->Bx; j++) {
			for (int k = 0; k < this->By; k++) {

				local = input_variables[0]->singleton(i, input_SW);
				if (local < temp)
					temp = local;

				local = input_variables[1]->singleton(j, input_Bx);
				if (local < temp)
					temp = local;

				local = input_variables[2]->singleton(k, input_By);
				if (local < temp)
					temp = local;
				switch (this->baseRuleMatriz[0][i][j][k]) {
				case 0:
					//A1
					if (output_variable_array[0] < temp)
						output_variable_array[0] = temp;
					break;
				case 1:
					//A2
					if (output_variable_array[1] < temp)
						output_variable_array[1] = temp;
					break;
				case 2:
					//A3
					if (output_variable_array[2] < temp)
						output_variable_array[2] = temp;
					break;
				case 10:
					//B1
					if (output_variable_array[3] < temp)
						output_variable_array[3] = temp;
					break;
				case 11:
					//B2
					if (output_variable_array[4] < temp)
						output_variable_array[4] = temp;
					break;
				case 12:
					//B3
					if (output_variable_array[5] < temp)
						output_variable_array[5] = temp;
					break;
				case 20:
					//C1
					if (output_variable_array[6] < temp)
						output_variable_array[6] = temp;
					break;
				case 21:
					//C2
					if (output_variable_array[7] < temp)
						output_variable_array[7] = temp;
					break;
				case 22:
					//C3
					if (output_variable_array[8] < temp)
						output_variable_array[8] = temp;
					break;
				case 30:
					//D1
					if (output_variable_array[9] < temp)
						output_variable_array[9] = temp;
					break;
				case 31:
					//D2
					if (output_variable_array[10] < temp)
						output_variable_array[10] = temp;
					break;
				case 32:
					//D3
					if (output_variable_array[11] < temp)
						output_variable_array[11] = temp;
					break;
				case 40:
					//E1
					if (output_variable_array[12] < temp)
						output_variable_array[12] = temp;
					break;
				case 41:
					//E2
					if (output_variable_array[13] < temp)
						output_variable_array[13] = temp;
					break;
				case 42:
					//E3
					if (output_variable_array[14] < temp)
						output_variable_array[14] = temp;
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

void GSRules::printBaseRuleMatriz() {
	for (int i = 0; i < this->SW; i++) {
		for (int j = 0; j < this->Bx; j++) {
			for (int k = 0; k < this->By; k++) {
				printf("%i : ", this->baseRuleMatriz[0][i][j][k]);
			}	
			printf("\n");
		}
		printf("\n");
	}
}