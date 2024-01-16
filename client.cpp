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

TCPclient c;

void InputGame();

void longrun();
//TODO: heck TCPclient::recieve first!
//TODO: Implement for waiting till server transmitted data
/*        while(1)
        {
                for(int i=0; i < maxDataSizeRecv_; i++){
                        dataRecv_[i] = '\0';
                 }*/

int main() {
	cout << "Client started" << endl;
	srand(time(NULL));
	string host = "localhost";
	string msg;
	
	//connect to host
	c.conn(host , 2022);
	string message = "";
	for(int n = 1; n<=10; n++){
		for(int m = 1; m <= 10; m++){
			message = "COORD[" + to_string(n) + ";" + to_string(m) + "]";
			c.sendData(message);
			message = c.receive(32);
			cout << message << endl;
			sleep(1);
		}
	}
	return 0;

	/*for(int n = 0; n<100; n++){
		//Send newGame to server
		longrun();
	}*/
}

void longrun(){
	int X = 1; 
	int Y = 1;
	int tries = 1;
	string msg = "";

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

