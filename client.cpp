/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <iostream>
#include <unistd.h> //contains various constants

#include "SIMPLESOCKET.H"

using namespace std;

void InputGame();

void longrun();

int main() {
	srand(time(NULL));
	TCPclient c;
	string host = "localhost";
	string msg;

	//connect to host
	c.conn(host , 2022);
}

void longrun(){
	int X = 1; 
	int Y = 1;
	int tries = 1;

	while(1){
		//*Kommunikation + Auswertung
		msg = to_string(X) + ";" + to_string(Y);
		c.sendData(msg);
		msg = c.receive(32);
		//*Augabe der Auswertung
		if(msg.compare("GAME OVER") == 0){
			cout << "Game Over!" << endl << "---------------------------------------------" << endl << "You've won after " << tries << " tries!" << endl;
			break;
		}
		else{
			cout << "Result: " << msg << endl;
		}
		//*"Algorithmus"
		if(X == 10){
			X = 1;
			Y++;
		}
		else{
			X++;
		}
		sleep(1);
		
	}
}

