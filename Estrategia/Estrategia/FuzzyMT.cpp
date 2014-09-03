#include "stdafx.h"
#include "FuzzyMT.h"

FuzzyMT::FuzzyMT(){

}

FuzzyMT::FuzzyMT(std::string file_name){

	this->fuzzy=(new MTRules(file_name));

	this->input_Variables.push_back((new Trapez(3)));
	this->input_Variables.push_back((new Trapez(5)));
	this->input_Variables.push_back((new Trapez(5)));

	//Orientação
	this->input_Variables[0]->addLabel(0, "otimo", this->input_Variables[0]->points(0, 0, 0, 45));
	this->input_Variables[0]->addLabel(1, "bom", this->input_Variables[0]->points(0, 45, 45, 90));
	this->input_Variables[0]->addLabel(2, "ruim", this->input_Variables[0]->points(45, 90, 90, 90));
	this->input_Variables[0]->setValues();
	
	// Distancia
	this->input_Variables[1]->addLabel(0, "muito perto", this->input_Variables[1]->points(0, 0, 0, 425));
	this->input_Variables[1]->addLabel(1, "perto", this->input_Variables[1]->points(0, 425, 425, 850));
	this->input_Variables[1]->addLabel(2, "medio", this->input_Variables[1]->points(425, 850, 850, 1275));
	this->input_Variables[1]->addLabel(3, "longe", this->input_Variables[1]->points(850, 1275, 1275, 1700));
	this->input_Variables[1]->addLabel(4, "muito longe", this->input_Variables[1]->points(1275, 1700, 1700, 1700));
	this->input_Variables[1]->setValues();

	// Distancia mais Proximo
	this->input_Variables[2]->addLabel(0, "muito + perto", this->input_Variables[2]->points(-1700, -1700, -200, -100));
	this->input_Variables[2]->addLabel(1, "+ perto", this->input_Variables[2]->points(-200, -100, -100, 0));
	this->input_Variables[2]->addLabel(2, "medio", this->input_Variables[2]->points(-100, 0, 0, 100));
	this->input_Variables[2]->addLabel(3, "+ longe", this->input_Variables[2]->points(0, 100, 100, 200));
	this->input_Variables[2]->addLabel(4, "muito + longe", this->input_Variables[2]->points(100, 200, 1700, 1700));
	this->input_Variables[2]->setValues();
	
	// SW
	this->output_variable = new Trapez(5);
	this->output_variable->addLabel(0, "super ataque", this->output_variable->points(1, 2, 2, 2));
	this->output_variable->addLabel(1, "ataque", this->output_variable->points(0, 1, 1, 2));
	this->output_variable->addLabel(2, "neutro", this->output_variable->points(-1, 0, 0, 1));
	this->output_variable->addLabel(3, "defesa", this->output_variable->points(-2, -1, -1, 0));
	this->output_variable->addLabel(4, "super defesa", this->output_variable->points(-2, -2, -2, -1));
	this->output_variable->setValues();

	//this->input_Variables[1]->addLabel(0, "dark", this->input_Variables[1]->points(0, 0, 1, 2.5));
	//this->input_Variables[1]->addLabel(1, "light", this->input_Variables[1]->points(1, 2.5, 5, 5));
	////    this->dayNigth->setValues(0, 5, 1000);
	//this->input_Variables[1]->setValues();


	//this->input_Variables[0]->addLabel(0, "negativo", this->input_Variables[0]->points(-5, -5, -5, 5));
	//this->input_Variables[0]->addLabel(1, "ok", this->input_Variables[0]->points(-1, 0, 0, 1));
	//this->input_Variables[0]->addLabel(2, "positivo", this->input_Variables[0]->points(-5, 5, 5, 5));
	////    this->horizontalError->setValues(-5, 5, 1000);
	//this->input_Variables[0]->setValues();


	//
	//this->output_variable = new Trapez(3);
	//this->output_variable->addLabel(0, "EW", this->output_variable->points(-5, -5, -3, -1));
	//this->output_variable->addLabel(1, "OK", this->output_variable->points(-1, 0, 0, 1));
	//this->output_variable->addLabel(2, "WE", this->output_variable->points(1, 3, 5, 5));
	////    this->motorHorizontal->setValues(-5, 5, 1000);
	//this->output_variable->setValues();

	this->fuzzy->printBaseRuleMatriz();

}

FuzzyMT::~FuzzyMT() {
	//dtor 
	//delete mtRules;
}

float FuzzyMT::defuzzyfier(std::vector<float> &inputs) {
	//this->mtRules->printBaseRuleMatriz();
	for(uint8_t i=0;i<this->input_Variables.size();i++){
		if(inputs[i] < this->input_Variables[i]->getVmin() || inputs[i] > this->input_Variables[i]->getVmax())
			return NULL;
	}
	return this->fuzzy->fuzzy(&inputs[0], this->input_Variables,this->output_variable);
}

//float Fuzzy::defuzzyfierVertical(const float &voltageError, const float &dayNightStatus) {
//    return this->verticalRules.fuzzy(voltageError, dayNightStatus, this->verticalError, this->dayNigth, this->motorVertical);
//}
