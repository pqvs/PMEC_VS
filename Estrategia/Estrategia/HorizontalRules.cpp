#include "HorizontalRules.h"
#include "DeFuzzy.h"
#include <iostream>
#include <memory>

HorizontalRules::HorizontalRules(const int ** const arr, const uint8_t errorLen, const uint8_t dayNigthLen) : baseRuleMatriz(arr), errorLen(errorLen), dayNigthLen(dayNigthLen) {
}

HorizontalRules::HorizontalRules(uint8_t errorLen, uint8_t dayNigthLen) : errorLen(errorLen), dayNigthLen(dayNigthLen) {
}

//void HorizontalRules::make(int **matriz) {
//    for (int i = 0; i < errorLen; i++) {
//        for (int j = 0; j < dayNigthLen; j++) {
//            this->HorizontalRulesMatriz[i][j] = matriz[i][j];
//        }
//    }
//}

HorizontalRules::~HorizontalRules() {
	//dtor
}

float HorizontalRules::fuzzy(float* inputs, std::vector<std::shared_ptr<FuzzyFunction>> &input_variables,FuzzyFunction *output_variable) {
	float errorPosition=inputs[0], dayNigthStatus=inputs[1];
	this->printBaseRuleMatriz();
	//Trapez *error=static_cast<Trapez*>((input_variables[0]).get()),*dayNigth=static_cast<Trapez*>((input_variables[1]).get());
	float *output_variable_array = (float*) calloc(output_variable->getnParticoes(), sizeof (float));
	for (int i = 0; i < output_variable->getnParticoes(); i++) {
		output_variable_array[i] = 0;
	}

	float temp = 1;
	float local = 0;
	for (int i = 0; i < errorLen; i++) {
		for (int j = 0; j < dayNigthLen; j++) {
			local = input_variables[0].get()->singleton(i, errorPosition);
			if (local < temp)
				temp = local;
			local = input_variables[1].get()->singleton(j, dayNigthStatus);
			if (local < temp)
				temp = local;
			switch (this->baseRuleMatriz[i][j]) {
			case 0:
				//EW | NS
				if (output_variable_array[0] < temp)
					output_variable_array[0] = temp;
				break;
			case 1:
				//OK
				if (output_variable_array[1] < temp)
					output_variable_array[1] = temp;
				break;
			case 2:
				//WE | SN
				if (output_variable_array[2] < temp)
					output_variable_array[2] = temp;
				break;
			default:
				break;
			}
			temp = 1;
		}
	}
	for (int i = 0; i < output_variable->getnParticoes(); i++) {
		printf("%f : ", output_variable_array[i]);
	}
	printf("\n");
	return DeFuzzy::centroidMax2(output_variable, output_variable_array);
}

void HorizontalRules::printBaseRuleMatriz() {
	for (int i = 0; i < errorLen; i++) {
		for (int j = 0; j < dayNigthLen; j++) {
			printf("%i : ", this->baseRuleMatriz[i][j]);
		}
		printf("\n");
	}

}
