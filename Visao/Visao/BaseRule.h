#ifndef BaseRule_H_INCLUDED
#define BaseRule_H_INCLUDED
#include "FuzzyFunction.h"
#include "DeFuzzy.h"
#include <stdint.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
using namespace std;

class BaseRule {
public:
    virtual void printBaseRuleMatriz()=0;
    virtual float fuzzy(float* inputs, std::vector<std::shared_ptr<FuzzyFunction>> &input_variables,FuzzyFunction *output_variable)=0;
protected:
private:
	virtual void generate_baseRuleMatrix(string file_name)=0;
};

#endif // BaseRule_H_INCLUDED
