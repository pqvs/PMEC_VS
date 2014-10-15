#pragma once
#include "BaseRule.h"
class VERules:public BaseRule
{
public:
	VERules();
	VERules::VERules( const VERules& other ) :
		baseRuleMatriz(other.baseRuleMatriz),Angulo( other.Angulo ), Distancia( other.Distancia )
	{}
	VERules(std::string file_name);
	~VERules();
	void printBaseRuleMatriz();
	float fuzzy(float* inputs, std::vector<std::shared_ptr<FuzzyFunction>> &input_variables,FuzzyFunction *output_variable);
protected:
private:
	int*** baseRuleMatriz;
	uint8_t Angulo,Distancia;
	void generate_baseRuleMatrix(string file_name);
};

