#pragma once
#include <random>
#include "Robo.h"
#include "Bola.h"
#include "Point.h"
#include <iostream>
#include "define.h"

#define FIELD_SIZE 30
#define CELL_SIZE 3
#define BALL_RANGE_DEF 5

using namespace std;
class EEMidForward
{
private:
	double sigma, speed, ballSpeed, ballOrientation, fitness,
			distanceEnemie0, distanceEnemie1, distanceEnemie2,
			distanceGoalkeeper, distanceAllie0, distanceAllie1,
			distanceCloserEnemie, distanceFromGoal, 
			expectedOri, expectedDist, robotExpectedSpeed,
			yFinalBall, erroAngular, erroLinear, sumErroLinear, sumErroAngular,
			fieldOrientation, robotOrientation;
	
	pt::Point expectedRobotPos, futureBallPos, goalPos, robotPosition,realGoalPos;
	vector<pt::Point> *lastPositions, *lastBallPositions;
	int positiveMutations, totalMutations, *pwm;
	default_random_engine generator;
	Robo *robot, **enemies, **allies;
	Objeto **objetos;
	Bola* ball;
	bool enemiesNear[3];

	

	void initializeField(void);
	
	void calcRepulsionField(void);
	void applyMutation(int* child);
	void calcExpected(int pwm[2]);
	void updateSigma(void);
	
	void calcErro(void);
	void calcPotField(void);
public:
	bool ballIn;
	double potField[FIELD_SIZE][FIELD_SIZE], oldPotField[FIELD_SIZE][FIELD_SIZE];
	EEMidForward(Objeto** objetos);
	void evolve(int* pwm);
	double evaluateFitness(int* child);
	static double distancePoints(pt::Point A, pt::Point B);
	void resetVars();
	void updateVars(void);
	~EEMidForward(void);
};

