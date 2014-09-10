#ifndef SERIALW_H_INCLUDED
#define SERIALW_H_INCLUDED
#define BAUDRATE 115200
#define TAM_BUFFER_BT 64

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <istream>
#include <sstream>
#include "Robo.h"
#include "TimerCounter.h"
using namespace std;

class SerialW {
private:
	void enviaPWM(string *PWMs);
	bool enviaDado(char *c);
	DWORD start();
	DWORD stop();
	bool isOpen();
	bool estabeleceConexao();

public:
TimerCounter t;
	SerialW();
	SerialW(const char *c);
	~SerialW(){
	};
	HANDLE hCom;
	DWORD nbytes;
	DWORD id;
	BOOL bPortReady;
	DCB dcb;
	COMMTIMEOUTS CommTimeouts;
	void send(Robo *robo_0, Robo *robo_1, Robo *robo_2);
	const string SERIAL_PORT;
	const string parada;
	bool envia(string str);

};


#endif // SERIALW_H_INCLUDED
