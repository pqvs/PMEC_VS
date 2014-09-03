#pragma once
#include "Fuzzy.h"
#include "GSRules.h"
class FuzzyGS:public Fuzzy
{
public:
	FuzzyGS();
    FuzzyGS(std::string file_name);
    ~FuzzyGS();
    float defuzzyfier(std::vector<float> &inputs);
protected:
private:
};

