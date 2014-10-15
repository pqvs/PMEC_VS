#pragma once
#include "BaseRule.h"
class VDRules:public BaseRule
{
public:
	VDRules();
	VDRules::VDRules( const VDRules& other ) :
		baseRuleMatriz(other.baseRuleMatriz),Angulo( other.Angulo ), Distancia( other.Distancia )
	{}
	VDRules(std::string file_name);
	~VDRules();
	void printBaseRuleMatriz();
	float fuzzy(float* inputs, std::vector<std::shared_ptr<FuzzyFunction>> &input_variables,FuzzyFunction *output_variable);
protected:
private:
	int*** baseRuleMatriz;
	uint8_t Angulo,Distancia;
	void generate_baseRuleMatrix(string file_name);
};

