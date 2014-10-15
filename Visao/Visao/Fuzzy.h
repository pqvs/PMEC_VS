#ifndef FUZZY_H_INCLUDED
#define FUZZY_H_INCLUDED
#include "FuzzyFunction.h"
#include "BaseRule.h"
#include <memory>

class Fuzzy{
public:
	virtual ~Fuzzy(){
	//	for (size_t i = 0; i < input_Variables.size(); ++i) {
	//	delete input_Variables[i]; // Calls ~object and deallocates *tmp[i]
	//}
	input_Variables.clear();
	}
protected:
	std::vector<std::shared_ptr<FuzzyFunction>> input_Variables;
    FuzzyFunction *output_variable;
private:
};


#endif
