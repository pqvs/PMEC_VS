#include "Fuzzy.h"
#include "MTRules.h"
class FuzzyMT: public Fuzzy {
public:
	FuzzyMT();
    FuzzyMT(std::string file_name);
    ~FuzzyMT();
    float defuzzyfier(std::vector<float> &inputs);
protected:
private:
};