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

int longrun();

int main() {
	cout << "Client started" << endl;	//Client started
	srand(time(NULL));				//Seed for random numbers
	string host = "localhost";		//Host to connect to
	unsigned char tries = 0;	//Number of tries, till game is over
	constexpr unsigned int Iterations = 1000; //Iterations of testing for statistics
	
	//connect to host
	cout << "Connecting to " << host << "..." << endl;
	c.conn(host , 2022);	//Connect to host on port 2022

	unsigned char triesArray[Iterations]; // Array to store the number of tries for each iteration

	for(int n = 0; n<Iterations; n++){
		triesArray[n] = 0;
		//Send newGame to server
		c.sendData("newGame");
		//Receive newGame from server
		string msg = c.receive(32);
		//cout << msg << endl;		 //Debug
		//Insert Strategy here;
		triesArray[n] = longrun();
		cout << "#" << n << ":  " << to_string(triesArray[n]) << endl;
	}
	//Calculate average
	//! Remove in final product... -> only for testing 
	int sum = 0;
	for (int i = 0; i < Iterations; i++) {
		sum += triesArray[i];
	}
	int average = sum / Iterations;
	cout << "-----------------------------------------"	<< endl;
	cout << "Average: " << average		<< endl;
	cout << "-----------------------------------------"	<< endl;
	sleep(100);
	//! ...till here

	return 0;
}

//* Kurze Erklärung zum Code:
int longrun(){
	string msg;	//String, welcher zwischen Client und Server ausgetauscht wird
	int tries = 0;	//Anzahl der Versuche, bis das Spiel vorbei ist, wird zu Beginn auf 0 gesetzt
	for(int n = 1; n<=10; n++){	//Einfacher Algorithmus, for->for = Abtasten Zeile für Zeile
		for(int m = 1; m <= 10; m++){
			tries++;	//Versuch wird bei jedem Schussversuch hochgezählt.
			msg = "COORD[" + to_string(m) + ";" + to_string(n) + "]";	//Nachricht wird zusammengesetzt
			//Der Server braucht dabei die Struktur: COORD[x;y], wobei x und y im Bereich [1,10]
			c.sendData(msg);	//Nachricht wird an Server gesendet
			msg = c.receive(32);	//Nachricht vom Server wird empfangen
			if(msg == "GAME OVER"){		//Wenn GAME OVER, 
				return tries;				//dann wird die Anzahl der Versuche zurückgegeben
			}
		}
	}	
	return -1;	//Wenn kein Game-Over erreicht wurde, wird -1 zurückgegeben, da dann ein Fehlversuch vorliegt.
}

