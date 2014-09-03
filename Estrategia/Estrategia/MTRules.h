#pragma once
#include "BaseRule.h"

class MTRules:public BaseRule
{
public:
	MTRules();
	//MTRules& MTRules::operator=( const MTRules& other ): baseRuleMatriz(other.baseRuleMatriz){
	//	//baseRuleMatriz = other.baseRuleMatriz;
	//	O = other.O;
	//	ND = other.ND;
	//	D=other.D;
	//	return *this;
	//}
	MTRules::MTRules( const MTRules& other ) :
		baseRuleMatriz(other.baseRuleMatriz),O( other.O ), ND( other.ND ), D( other.D )
	{}
	MTRules(std::string file_name);
	~MTRules();
	void printBaseRuleMatriz();
	float fuzzy(float* inputs, std::vector<FuzzyFunction*> &input_variables,FuzzyFunction *output_variable);
protected:
private:
	int**** baseRuleMatriz;
	uint8_t O,ND,D;
	void generate_baseRuleMatrix(string file_name);
};

