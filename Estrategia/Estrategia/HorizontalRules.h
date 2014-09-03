#include "BaseRule.h"
class HorizontalRules{
public:
	HorizontalRules(const int ** const arr, const uint8_t errorLen, const uint8_t dayNigthLen);
    HorizontalRules(uint8_t errorLen, uint8_t dayNigthLen);
	//HorizontalRules();
	~HorizontalRules();
	void printBaseRuleMatriz();
    float fuzzy(float* inputs, std::vector<std::shared_ptr<FuzzyFunction>> &input_variables,FuzzyFunction *output_variable);
protected:
private:
	const int **baseRuleMatriz;
    const uint8_t errorLen;
    const uint8_t dayNigthLen;
};