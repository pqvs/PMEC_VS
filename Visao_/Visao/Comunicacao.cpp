// Comunicacao.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Robo.h"
#include "SerialW.h"

int _tmain(int argc, _TCHAR* argv[])
{
	SerialW s= SerialW("\\\\.\\COM30");
	Robo *robos=new Robo[3];
	robos[0].velocity_left=100;
	robos[0].velocity_rigth=82;
	robos[1].velocity_left=3;
	robos[1].velocity_rigth=4;
	robos[2].velocity_left=5;
	robos[2].velocity_rigth=6;

	/*robos[0].velocity_left=-255;
	robos[0].velocity_rigth=-255;
	robos[1].velocity_left=-255;
	robos[1].velocity_rigth=-255;
	robos[2].velocity_left=-255;
	robos[2].velocity_rigth=-255;*/
	s.send(&robos[0],&robos[1],&robos[2]);
	return 0;
}

