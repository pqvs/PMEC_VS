#pragma once
#include "BaseRule.h"
class GSRules:public BaseRule
{
public:
	GSRules();
	GSRules::GSRules( const GSRules& other ) :
		baseRuleMatriz(other.baseRuleMatriz),SW( other.SW ), Bx( other.Bx ), By( other.By )
	{}
	GSRules(std::string file_name);
	~GSRules();
	void printBaseRuleMatriz();
	float fuzzy(float* inputs, std::vector<FuzzyFunction*> &input_variables,FuzzyFunction *output_variable);
protected:
private:
	int**** baseRuleMatriz;
	uint8_t SW,Bx,By;
	void generate_baseRuleMatrix(string file_name);
};

