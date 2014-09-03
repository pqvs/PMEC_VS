#include "FuzzySunTraker.h"

FuzzySunTraker::FuzzySunTraker(const int ** const arr, const uint8_t errorLen, const uint8_t dayNigth) : horizontalRules(arr, errorLen, dayNigth){
	//    this->horizontalRules.make(arr);
	//    this->verticalRules.make(arr);


	this->input_Variables.push_back(std::shared_ptr<FuzzyFunction>(new Trapez(3)));
	this->input_Variables.push_back(std::shared_ptr<FuzzyFunction>(new Trapez(2)));


	this->input_Variables[1]->addLabel(0, "dark", this->input_Variables[1]->points(0, 0, 1, 2.5));
	this->input_Variables[1]->addLabel(1, "light", this->input_Variables[1]->points(1, 2.5, 5, 5));
	//    this->dayNigth->setValues(0, 5, 1000);
	this->input_Variables[1]->setValues();


	this->input_Variables[0]->addLabel(0, "negativo", this->input_Variables[0]->points(-5, -5, -5, 5));
	this->input_Variables[0]->addLabel(1, "ok", this->input_Variables[0]->points(-1, 0, 0, 1));
	this->input_Variables[0]->addLabel(2, "positivo", this->input_Variables[0]->points(-5, 5, 5, 5));
	//    this->horizontalError->setValues(-5, 5, 1000);
	this->input_Variables[0]->setValues();


	//Trapez dayNigth = Trapez(2);
	//dayNigth.addLabel(0, "dark", dayNigth.points(0, 0, 1, 2.5));
	//dayNigth.addLabel(1, "light", dayNigth.points(1, 2.5, 5, 5));
	////    this->dayNigth->setValues(0, 5, 1000);
	//dayNigth.setValues();

	//Trapez horizontalError = Trapez(3);
	//horizontalError.addLabel(0, "negativo", horizontalError.points(-5, -5, -5, 5));
	//horizontalError.addLabel(1, "ok", horizontalError.points(-1, 0, 0, 1));
	//horizontalError.addLabel(2, "positivo", horizontalError.points(-5, 5, 5, 5));
	////    this->horizontalError->setValues(-5, 5, 1000);
	//horizontalError.setValues();

	//this->verticalError = new Trapez(3);
	//this->verticalError->addLabel(0, "negativo", this->verticalError->points(-5, -5, -5, 5));
	//this->verticalError->addLabel(1, "ok", this->verticalError->points(-1.5, 0, 0, 1.5));
	//this->verticalError->addLabel(2, "positivo", this->verticalError->points(-5, 5, 5, 5));
	////    this->verticalError->setValues(-5, 5, 1000);
	//this->verticalError->setValues();

	this->output_variable = new Trapez(3);
	this->output_variable->addLabel(0, "EW", this->output_variable->points(-5, -5, -3, -1));
	this->output_variable->addLabel(1, "OK", this->output_variable->points(-1, 0, 0, 1));
	this->output_variable->addLabel(2, "WE", this->output_variable->points(1, 3, 5, 5));
	//    this->motorHorizontal->setValues(-5, 5, 1000);
	this->output_variable->setValues();

	//this->motorVertical = new Trapez(3);
	//this->motorVertical->addLabel(0, "SN", this->motorVertical->points(-5, -3, -3, -1));
	//this->motorVertical->addLabel(1, "OK", this->motorVertical->points(-1, 0, 0, 1));
	//this->motorVertical->addLabel(2, "NS", this->motorVertical->points(1, 3, 3, 5));
	////    this->motorVertical->setValues(-5, 5, 1000);
	//this->motorVertical->setValues();
}

FuzzySunTraker::~FuzzySunTraker() {
	//dtor   
	//for (int i = 0; i < input_Variables.size(); ++i) {
	//	delete input_Variables[i]; // Calls ~object and deallocates *tmp[i]
	//}
	input_Variables.clear();
	delete output_variable;
}


float FuzzySunTraker::defuzzyfier(std::vector<float> &inputs) {
	return this->horizontalRules.fuzzy(&inputs[0], this->input_Variables,this->output_variable);
}

//float Fuzzy::defuzzyfierVertical(const float &voltageError, const float &dayNightStatus) {
//    return this->verticalRules.fuzzy(voltageError, dayNightStatus, this->verticalError, this->dayNigth, this->motorVertical);
//}
