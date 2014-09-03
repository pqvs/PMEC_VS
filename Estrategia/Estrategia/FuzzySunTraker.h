#include "HorizontalRules.h"
#include "Fuzzy.h"
class FuzzySunTraker: public Fuzzy {
public:
    FuzzySunTraker(const int ** const arr, const uint8_t errorLen, const uint8_t dayNigth);
    ~FuzzySunTraker();
    float defuzzyfier(std::vector<float> &inputs);
protected:
private:
    HorizontalRules horizontalRules;
};