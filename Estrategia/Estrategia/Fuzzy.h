#ifndef FUZZY_H_INCLUDED
#define FUZZY_H_INCLUDED
#include "FuzzyFunction.h"
#include "BaseRule.h"
#include <memory>
#include "stdafx.h"
class Fuzzy{
public:
	Fuzzy(){}
	Fuzzy(const Fuzzy& src) : input_Variables(src.input_Variables), output_variable(src.output_variable),fuzzy(src.fuzzy) {}
	Fuzzy& operator=(const Fuzzy& rhs)
	{
		input_Variables=rhs.input_Variables;
		output_variable=rhs.output_variable;
		fuzzy=rhs.fuzzy;
		return *this;
	}

	virtual ~Fuzzy(){
		//	for (size_t i = 0; i < input_Variables.size(); ++i) {
		//	delete input_Variables[i]; // Calls ~object and deallocates *tmp[i]
		//}
		input_Variables.clear();
	}
protected:
	std::vector<FuzzyFunction*> input_Variables;
	FuzzyFunction *output_variable;
	BaseRule* fuzzy;
private:
};


#endif
