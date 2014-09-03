#include "stdafx.h"
#include "FuzzyGS.h"


FuzzyGS::FuzzyGS()
{
}

FuzzyGS::FuzzyGS(std::string file_name){

	this->fuzzy=(new GSRules(file_name));

	this->input_Variables.push_back((new Trapez(5)));
	this->input_Variables.push_back((new Trapez(5)));
	this->input_Variables.push_back((new Trapez(3))); 


	this->input_Variables[0]->addLabel(0, "super ataque", this->input_Variables[0]->points(1, 2, 2, 2));
	this->input_Variables[0]->addLabel(1, "ataque", this->input_Variables[0]->points(0, 1, 1, 2));
	this->input_Variables[0]->addLabel(2, "neutro", this->input_Variables[0]->points(-1, 0, 0, 1));
	this->input_Variables[0]->addLabel(3, "defesa", this->input_Variables[0]->points(-2, -1, -1, 0));
	this->input_Variables[0]->addLabel(4, "super defesa", this->input_Variables[0]->points(-2, -2, -2, -1));
	this->input_Variables[0]->setValues();


	this->input_Variables[1]->addLabel(0, "muito proximo", this->input_Variables[1]->points(0, 420, 420, 420));
	this->input_Variables[1]->addLabel(1, "proximo", this->input_Variables[1]->points(0, 420, 420, 800));
	this->input_Variables[1]->addLabel(2, "meio campo", this->input_Variables[1]->points(420, 800, 800, 1180));
	this->input_Variables[1]->addLabel(3, "longe", this->input_Variables[1]->points(800, 1180, 1180, 1500));
	this->input_Variables[1]->addLabel(4, "muito longe", this->input_Variables[1]->points(1180, 1500, 1500, 100));
	this->input_Variables[1]->setValues();

	// ATEN��O COM A ORIENTA��O DO CAMPO
	this->input_Variables[2]->addLabel(0, "direita", this->input_Variables[2]->points(650, 900, 1300, 1300));
	this->input_Variables[2]->addLabel(1, "meio", this->input_Variables[2]->points(200, 650, 650, 1100));
	this->input_Variables[2]->addLabel(2, "esquerda", this->input_Variables[2]->points(0, 0, 400, 650));
	this->input_Variables[2]->setValues();

	this->output_variable = new Trapez(15);
	this->output_variable->addLabel(0, "a1", this->output_variable->points(6, 7, 7.5, 7.5));
	this->output_variable->addLabel(1, "a2", this->output_variable->points(5, 6, 6, 7));
	this->output_variable->addLabel(2, "a3", this->output_variable->points(4, 5, 5, 6));
	this->output_variable->addLabel(3, "b3", this->output_variable->points(3, 4, 4, 5));
	this->output_variable->addLabel(4, "b2", this->output_variable->points(2, 3, 3, 4));
	this->output_variable->addLabel(5, "b1", this->output_variable->points(1, 2, 2, 3));
	this->output_variable->addLabel(6, "c1", this->output_variable->points(0, 1, 1, 2));
	this->output_variable->addLabel(7, "c2", this->output_variable->points(-1, 0, 0, 1));
	this->output_variable->addLabel(8, "c3", this->output_variable->points(-2, -1, -1, 0));
	this->output_variable->addLabel(9, "d3", this->output_variable->points(-3, -2, -2, -1));
	this->output_variable->addLabel(10, "d2", this->output_variable->points(-4, -3, -3, -2));
	this->output_variable->addLabel(11, "d1", this->output_variable->points(-5, -4, -4, -3));
	this->output_variable->addLabel(12, "e1", this->output_variable->points(-6, -5, -5, -4));
	this->output_variable->addLabel(13, "e2", this->output_variable->points(-7, -6, -6, -5));
	this->output_variable->addLabel(14, "e3", this->output_variable->points(-7.5, -7.5, -7, -6));
	this->output_variable->setValues();
}

FuzzyGS::~FuzzyGS()
{
}

float FuzzyGS::defuzzyfier(std::vector<float> &inputs) {
	for(uint8_t i=0;i<this->input_Variables.size();i++){
		if(inputs[i] < this->input_Variables[i]->getVmin() || inputs[i] > this->input_Variables[i]->getVmax())
			return NULL;
	}
	return this->fuzzy->fuzzy(&inputs[0], this->input_Variables,this->output_variable);
}