// CampoNavegacao.cpp : main project file.
#include "stdafx.h"
#include <iostream>
#include <windows.h>

double PCFreq = 0.0;
__int64 CounterStart = 0;

using namespace std;

void StartCounter(){
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
	std::cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter(){
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}


int run()
{
	int X = 10, Y = 20;
	int OX1 = 3, OY1 = 16;
	int OX2 = 8, OY2 = 4;
	int OX3 = 10, OY3 = 18;
	int OX4 = 37, OY4 = 16;
	int OX5 = 32, OY5 = 29;


	int linhas = 33+2;
	int colunas = 38+2;
	// Inicialização das matrizes
	double** potencialCampo = new double*[linhas];
	double** potencialCampoOld = new double*[linhas];
	for (int i = 0; i < linhas; i++) {
		potencialCampo[i] = new double[colunas];
		potencialCampoOld[i] = new double[colunas];
	}

	for(int loops = 0; loops < 200; loops++){
		// Preenche matriz com zeros
		for (int i = 0; i < linhas; i++) {
			for (int j = 0; j < colunas; j++) {
					potencialCampo[i][j] = 0;
			}
		}
		// Seta as laterais do campo com potencial 1;
		int range = (linhas > colunas) ? linhas : colunas;
		for (int i = 0; i < range; i++) {
			if(i < linhas){
				potencialCampo[i][0] = 1;
				potencialCampo[i][colunas-1] = 1;
			}
			if(i < colunas){
				potencialCampo[0][i] = 1;
				potencialCampo[linhas-1][i] = 1;
			}
		}

		//seta obstáculos com potencial 1;
		potencialCampo[OY1][OX1] = 1;
		potencialCampo[OY2][OX2] = 1;
		potencialCampo[OY3][OX3] = 1;
		potencialCampo[OY4][OX4] = 1;
		potencialCampo[OY5][OX5] = 1;

		// Copia o valor de campo para campoOld
		for (int i = 0; i < linhas; i++) {
			for (int j = 0; j < colunas; j++) {
				potencialCampoOld[i][j] = potencialCampo[i][j];
			}
		}
	
		// -------Cálculo do campo potencial------------
		float p = 0.5*(cos(3.14159265/linhas) + cos(3.14159265/colunas));
		float w  = 2/(1+sqrt(1-p*p));

		double eanterior = 0, eatual;
		int t;
	
		StartCounter();
		for (t = 0; t < 1500; t++){
			eatual = 0;
			for (int i = 1; i < linhas-1; i++){
				for (int j = 1; j < colunas-1; j++){
					if(i == X && j == Y)
						continue;
					if((i == OX1 && j == OY1) || (i == OX2 && j == OY2) || (i == OX3 && j == OY3) || (i == OX4 && j == OY4) || (i == OX5 && j == OY5) )//|| (i == OX6 && j == OY6) || (i == OX7 && j == OY7))
						continue;
					potencialCampo[i][j] = potencialCampoOld[i][j]+w*(potencialCampo[i][j-1]+potencialCampoOld[i][j+1]+potencialCampo[i-1][j]+potencialCampoOld[i+1][j]-4*potencialCampoOld[i][j])/4;
					eatual = eatual + abs(potencialCampo[i][j]-potencialCampoOld[i][j]);
				}
			}

			// Copia o valor de campo para campoOld
			for (int i = 0; i < linhas; i++) {
				for (int j = 0; j < colunas; j++) {
					potencialCampoOld[i][j] = potencialCampo[i][j];
				}
			}

			if(abs(eatual - eanterior) < 10.0E-12)
				break;
		
			eanterior = eatual;
		}
		// ---------------------------------------------
		std::cout<<"Tempo: "<< GetCounter();
		std::cout<<" ,iteracoes: "<<t<<std::endl;
	
	}




	//// Print Matriz
	//for (int i = 0; i < linhas; i++) {
	//	for (int j = 0; j < colunas; j++) {
	//			cout<<potencialCampo[i][j];
	//	}
	//	cout<<endl;
	//}

	//// Print Matriz old
	//cout<<endl<<endl<<"Matriz OLD;"<<endl;
	//for (int i = 0; i < linhas; i++) {
	//	for (int j = 0; j < colunas; j++) {
	//			cout<<potencialCampoOld[i][j];
	//	}
	//	cout<<endl;
	//}
	Sleep(40000);
    return 0;

}


