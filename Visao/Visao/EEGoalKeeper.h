#pragma once
#include <random>
#include "Robo.h"
#include "Bola.h"
#include "Point.h"
#include <iostream>
#include "define.h"

using namespace std;

class EEGoalKeeper
{
private:
	double sigma, speed, ballSpeed, ballOrientation, fitness,
			distanceEnemie0, distanceEnemie1, distanceEnemie2,
			distanceGoalkeeper, distanceAllie0, distanceAllie1,
			distanceCloserEnemie, distanceFromGoal, 
			expectedOri, expectedDist, robotExpectedSpeed,
			yFinalBall, erroAngular, erroLinear;
	pt::Point expectedRobotPos, futureBallPos;
	vector<pt::Point> *lastPositions, *lastBallPositions;
	int positiveMutations, totalMutations, *pwm;
	default_random_engine generator;
	Robo *robot, **enemies, **allies;
	Bola* ball;
	void applyMutation(int* child);
	void calcExpected(int pwm[2]);
	void updateSigma(void);
	void predicaoDaBola(void);
	void updateVars(void);
	void calcErro(void);
public:
	EEGoalKeeper(Objeto** objetos);
	void evolve(int* pwm);
	double evaluateFitness(int* child);
	static double distancePoints(pt::Point A, pt::Point B);
	void resetVars();
	~EEGoalKeeper(void);
};

