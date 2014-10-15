#include "FuzzyVD.h"


FuzzyVD::FuzzyVD()
{
}

FuzzyVD::FuzzyVD(std::string file_name){

	this->vdRules=std::shared_ptr<VDRules>(new VDRules(file_name));

	this->input_Variables.push_back(std::shared_ptr<FuzzyFunction> (new Trapez(5)));
	this->input_Variables.push_back(std::shared_ptr<FuzzyFunction>(new Trapez(5))); 
	    
	//Memberships angulo - input
	this->input_Variables[0]->addLabel(0, "E", this->input_Variables[0]->points(-100, -90, -50, -20));
    this->input_Variables[0]->addLabel(1, "CE", this->input_Variables[0]->points(-50, -20, -20, 0));
    this->input_Variables[0]->addLabel(2, "C", this->input_Variables[0]->points(-20, 0, 0, 20));
    this->input_Variables[0]->addLabel(3, "CD", this->input_Variables[0]->points(0, 20, 20, 50));
    this->input_Variables[0]->addLabel(4, "D", this->input_Variables[0]->points(20, 50, 90, 100));
    this->input_Variables[0]->setValues();
    
	//Memberships distancia - input
	this->input_Variables[1]->addLabel(0, "muito proximo", this->input_Variables[1]->points(-1, 0, 200, 600));
    this->input_Variables[1]->addLabel(1, "proximo", this->input_Variables[1]->points(200, 600, 600, 1000));
    this->input_Variables[1]->addLabel(2, "meio campo", this->input_Variables[1]->points(600, 1000, 1000, 1400));
    this->input_Variables[1]->addLabel(3, "longe", this->input_Variables[1]->points(1000, 1400, 1400, 1800));
    this->input_Variables[1]->addLabel(4, "muito longe", this->input_Variables[1]->points(1400, 1800, 2000, 2100));
    this->input_Variables[1]->setValues();

	//Memberships ve e vd  - output
    this->output_variable = new Trapez(5);
    this->output_variable->addLabel(0, "Muito Devagar", this->output_variable->points(-25, 0, 0, 25));
    this->output_variable->addLabel(1, "Devagar", this->output_variable->points(0, 25, 25, 50));
    this->output_variable->addLabel(2, "Medio", this->output_variable->points(25, 50, 50, 75));
    this->output_variable->addLabel(3, "Rapido", this->output_variable->points(50, 75, 75, 100));
    this->output_variable->addLabel(4, "Muito Rapido", this->output_variable->points(75, 100, 100, 125));
    this->output_variable->setValues();



}

FuzzyVD::~FuzzyVD()
{
}

float FuzzyVD::defuzzyfier(std::vector<float> &inputs) {
	for(uint8_t i=0;i<this->input_Variables.size();i++){
		if(inputs[i] < this->input_Variables[i]->getVmin() || inputs[i] > this->input_Variables[i]->getVmax())
			return NULL;
	}
	return this->vdRules->fuzzy(&inputs[0], this->input_Variables,this->output_variable);
}