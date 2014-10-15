#pragma once
#include "fuzzy.h"
#include "VDRules.h"
class FuzzyVD:public Fuzzy
{
public:
	FuzzyVD();
    FuzzyVD(std::string file_name);
    ~FuzzyVD();
    float defuzzyfier(std::vector<float> &inputs);
protected:
private:
	std::shared_ptr<VDRules> vdRules;
};

