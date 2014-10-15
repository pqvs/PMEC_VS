#pragma once
#include "fuzzy.h"
#include "VERules.h"
class FuzzyVE:public Fuzzy
{
public:
	FuzzyVE();
    FuzzyVE(std::string file_name);
    ~FuzzyVE();
    float defuzzyfier(std::vector<float> &inputs);
protected:
private:
	std::shared_ptr<VERules> veRules;
};

