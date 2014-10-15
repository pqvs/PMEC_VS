#include "VERules.h"


VERules::VERules(std::string file_name)
{
	generate_baseRuleMatrix(file_name);
	//this->printBaseRuleMatriz();

}


VERules::~VERules()
{
}

VERules::VERules():baseRuleMatriz(nullptr){


}

void VERules::generate_baseRuleMatrix(std::string file_name){
	//Read from file_name
	ifstream in(file_name);
	if(!in)
	{
		printf( "Erro na abertura do arquivo");
		getchar();
		exit(0);
	}
	string line, field;
	vector< vector<int> > array2d;  // the 2D array
	vector<int> array1d;                // array of values for one line only
	int count=0;
	while ( getline(in,line) )    // get next line in file
	{
		array1d.clear();
		stringstream ss(line);
	
		while (getline(ss,field,';'))  // break line into comma delimitted fields
		{
			array1d.push_back(atoi(field.c_str()));  // add each field to the 1D array
		}

		array2d.push_back(array1d);  // add the 1D array to the 2D array
	}
	array1d.clear();
	//---------------------//
	this->Angulo=array2d.size();
	this->Distancia=array2d[this->Angulo-1].size();
	this->baseRuleMatriz=new int**;
	this->baseRuleMatriz[0]= new int*[this->Angulo];
	for (int i = 0; i < this->Angulo; i++) {
		this->baseRuleMatriz[0][i] = new int[this->Distancia];
	}


	for (int i = 0; i < this->Angulo; i++) {
		for (int j = 0; j < this->Distancia; j++) {
				this->baseRuleMatriz[0][i][j]=array2d[i][j];
		}
	}
}

float VERules::fuzzy(float* inputs, std::vector<std::shared_ptr<FuzzyFunction>> &input_variables,FuzzyFunction *output_variable){
	float input_Angulo=inputs[0],input_Distancia=inputs[1];

	float *output_variable_array = (float*) calloc(output_variable->getnParticoes(), sizeof (float));
	for (int i = 0; i < output_variable->getnParticoes(); i++) {
		output_variable_array[i] = 0;
	}
	float temp = 1;
	float local = 0;
	int conta=1;
	for (int i = 0; i < this->Angulo; i++) {
		for (int j = 0; j < this->Distancia; j++) {
				local = input_variables[0].get()->singleton(i, input_Angulo);
				if (local < temp)
					temp = local;
				
				//printf("%d: Local A:%f ",conta++,local);

				local = input_variables[1].get()->singleton(j, input_Distancia);
				if (local < temp)
					temp = local;

				//printf("- Local D:%f - temp: %f\n",local,temp);

				switch (this->baseRuleMatriz[0][j][i]) {
				case 1:
					//Muito Devagar
					if (output_variable_array[0] < temp)
						output_variable_array[0] = temp;
					break;
				case 2:
					//Devagar
					if (output_variable_array[1] < temp)
						output_variable_array[1] = temp;
					break;
				case 3:
					//Medio
					if (output_variable_array[2] < temp)
						output_variable_array[2] = temp;
					break;
				case 4:
					//Rapido
					if (output_variable_array[3] < temp)
						output_variable_array[3] = temp;
					break;
				case 5:
					//Muito Rapido
					if (output_variable_array[4] < temp)
						output_variable_array[4] = temp;
					break;
				default:
					break;
				}
				temp = 1;
		}
	}
	/*for (int i = 0; i < output_variable->getnParticoes(); i++) {
		printf("%f : ", output_variable_array[i]);
	}
	printf("\n");*/
	return DeFuzzy::centroidMax2(output_variable, output_variable_array);
}

void VERules::printBaseRuleMatriz() {
	for (int i = 0; i < this->Angulo; i++) {
		for (int j = 0; j < this->Distancia; j++) {
				printf("%i : ", this->baseRuleMatriz[0][i][j]);
		}
		printf("\n");
	}
}