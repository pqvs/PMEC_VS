#include "stdafx.h"
#include "SerialW.h"

/*
-------------------------------------------------------------------------------
|    0bX10101 cabe�alho da transmiss�o                                        |
|    0bX10101xx00000000 endere�amento do robo                                 |
|    0bX1010100xx000000 instru��o do motor 1 - se frente 10 ou traz 01 = 1    |  com paridade se frente 1 se traz 0, ou seja, apenas um bit para o motor
|    0bX101010000xx0000 instru��o do motor 2 - se frente 10 ou traz 01 = 1    |
|    0bX10101000000xx00 PWM do motor 1                                        |
|    0bX1010100000000xx PWM do motor 2                                        |
|    A id�ia � que se o robo receber algo diferente disso e tiver             |
|    passado o tempo de execu��o da ultima instru��o, ele para                |
|    cada robo vai ter uma mascara propria:                                   |
|                       ROBO1  = 0b0000000100000000,                          |
|          ROBO2 = 0b0000001000000000, ROBO3 = 0b0000001100000000             |
|                                                                             |
| obs: caso com paridade o MSB deve ser substituido pelo bit de paridade,     |
|                    logo, o primeiro zero poder� ser retirado                |
-------------------------------------------------------------------------------
*/

/*
const char      HEADLINE        = 0b1010100,
ROBO1           = 0b0000001,
ROBO2           = 0b0000010,
ROBO3           = 0b0000011,
MOTOR1_TRAZ     = 0b0000000,
MOTOR1_FRENTE   = 0b1000000,
MOTOR2_TRAZ     = 0b0010000,
MOTOR2_FRENTE   = 0b0100000,
PWM1_verylow    = 0b0000000,
PWM1_low        = 0b0000100,
PWM1_high       = 0b0001000,
PWM1_veryhigh   = 0b0001100,
PWM2_verylow    = 0b0000000,
PWM2_low        = 0b0000001,
PWM2_high       = 0b0000010,
PWM2_veryhigh   = 0b0000011,
ENQ             = 0b0000101,
ETX             = 0b0000011;
*/

SerialW::SerialW() 
{
}
SerialW::SerialW(const char *c) : SERIAL_PORT(c),parada("@0;0;0.0;0;0.0;0;0.#")
{
	//t= TimerCounter();
	estabeleceConexao();
}

DWORD SerialW::start() {
	int len = strlen(SERIAL_PORT.c_str()) + 1;
	wchar_t *w_msg = new wchar_t[len];
	memset(w_msg, 0, len);
	MultiByteToWideChar(CP_ACP, NULL, SERIAL_PORT.c_str(), -1, w_msg, len);
	//configura��o
	hCom = CreateFile(w_msg, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hCom == INVALID_HANDLE_VALUE) {
		return 0; //Erro ao tentar abrir a porta especificada.
	}
	bPortReady = SetupComm(hCom, 3, 1200); // set buffer sizes
	bPortReady = GetCommState(hCom, &dcb);
	if (!GetCommState(hCom, &dcb)) {
		printf("Erro na leitura de DCB.\n");
		return 0; //// Erro na leitura de DCB.
	} else {
		dcb.BaudRate = BAUDRATE;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.fAbortOnError = TRUE;
		// set XON/XOFF
		dcb.fOutX = FALSE; // XON/XOFF off for transmit
		dcb.fInX = FALSE; // XON/XOFF off for receive
		// set RTSCTS
		dcb.fOutxCtsFlow = FALSE; // turn off CTS flow control
		dcb.fRtsControl = RTS_CONTROL_DISABLE; //
		// set DSRDTR
		dcb.fOutxDsrFlow = FALSE; // turn off DSR flow control
		dcb.fDtrControl = DTR_CONTROL_DISABLE; //
	}
	if (!SetCommState(hCom, &dcb)) {
		printf("Erro nas configuracoes.\n");
		return 0; //// Erro na leitura de DCB.
	}
	return 1;
}

DWORD SerialW::stop() {
	WriteFile(hCom, SerialW::parada.c_str(), strlen(SerialW::parada.c_str()), &nbytes, NULL);
	PurgeComm(hCom, PURGE_TXABORT);
	CloseHandle(hCom);
	return true;
}

bool SerialW::isOpen() {
	if (GetCommState(hCom, &dcb)) return true;
	else return false;
}

bool SerialW::enviaDado(char *c) {
	//cout<<c<<endl;
	if (WriteFile(hCom, c, strlen(c), &nbytes, NULL)) return true;
	else return false;

}


void SerialW::enviaPWM(string *PWMs) {
	/*
	int size;
	size = sizeof PWMs / sizeof (char);
	char str[size];
	str[0] = PWMs[0];
	strcat(str, ";");
	for (int i = 2; i < size * 2; i++) {
	str[i] = PWMs[i - 1];
	strcat(str, ";");
	}
	PWMs = str;
	enviaDado(PWMs);
	*/

	char aux[TAM_BUFFER_BT]={};
	int size=0; 
	aux[size]='@';size++;
	for(int robos=0;robos<3;robos++){

		for (int i = 0; i < PWMs[robos].length()-1; i++,size++)
			aux[size] = PWMs[robos].at(i);


		aux[size]=';';
		int size_int=log10(PWMs[robos].length()) + 1;//pega o numero de digitos do inteiro

		int size_aux=PWMs[robos].length();
		size++;
		int size_size_aux=log10(size_int) + size_int -1 + size;
		char c[2];
		for (int i = 0; i < size_int; i++, size_aux/=10, size++,
			size_size_aux--){
				_itoa_s(size_aux%10,c,2,10);
				aux[size_size_aux]=*c;
		}
		aux[size] = '.';
		size++;
	}
	
	aux[size] = '#';
	//cout<<aux<<endl;
	//while(1){
		//Sleep(1);
		if (!enviaDado(aux)) {
			stop();
			while (start());	
			enviaDado(aux);
		}
	//}
}


void SerialW::send(Robo *robo_0, Robo *robo_1, Robo *robo_2){

	string PWMs[3];
	stringstream msg_sst;
	msg_sst<<robo_0->velocity_right<<";"<<robo_0->velocity_left<<";"<<50<<";";
	PWMs[0]=msg_sst.str();
	msg_sst.str(std::string());
	msg_sst<<robo_1->velocity_right<<";"<<robo_1->velocity_left<<";"<<50<<";";
	PWMs[1]=msg_sst.str();
	msg_sst.str(std::string());
	msg_sst<<robo_2->velocity_right<<";"<<robo_2->velocity_left<<";"<<50<<";";
	PWMs[2]=msg_sst.str();
	msg_sst.str(std::string());
	this->enviaPWM(PWMs);
}

bool SerialW::estabeleceConexao() {
	while (true)
		if (!start()) {
			system("cls");
			cout << "Erro ao especificar porta. Robo COM" << id << " nao conectado" << endl << "...";
			Sleep(200);
		} else
			break;
		return true;
}

bool SerialW::envia(string str) {
	char *aux=new char[TAM_BUFFER_BT];
	for (int i = 0; i < str.length(); i++)
		aux[i] = str.at(i);
	aux[str.length()] = '\0';
	if (!enviaDado(aux)) {
		stop();
		while (start());
		enviaDado(aux);
	}
	delete aux;
	return true;
}

