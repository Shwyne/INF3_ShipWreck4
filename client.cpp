/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 *  Modified: WiSe23_24
 * 		by Group: 
 * 		- 1: 	nkla
 * 		- 2: 	pfil
 * 		- 3: 	tste
 */

#include <string>
#include <iostream>
#include <unistd.h> //contains various constants
#include <fstream>
#include <random>
#include <set>

#include "SIMPLESOCKET.H"

using namespace std;

TCPclient c;

//*Variables and Constexpr:
unsigned char tries = 0;	//Number of tries, till game is over
constexpr unsigned int Iterations = 1000; //Iterations of testing for statistics
bool debug = false;	//Debug mode, if true, then after every shot the program will pause for a second

int longrun();
int randomRun();
int HuntRandom();

int main() {
	//*Setup Client
	cout << "Client started" << endl;	//Client started
	srand(time(NULL));				//Seed for random numbers
	string host = "localhost";		//Host to connect to
	
	//*connect to host
	cout << "Connecting to " << host << "..." << endl;
	c.conn(host , 2022);	//Connect to host on port 2022

	unsigned char triesArray[Iterations]; //! Temp Array for Average calculation
	
	//*Open file for writing
	ofstream ExportData;
	ExportData.open("ExportData.csv");
	if(ExportData.is_open()){
		cout << "DataFile opened successfully, Program can export now" << endl;
	}
	else{
		cout << "Error opening Datafile" << endl;
	}
	//*Choose Debug mode
	cout << "Debug mode? (y/n)" << endl;
	char debugMode;
	cin >> debugMode;
	if(debugMode == 'y'){
		debug = true;
		cout << "Debug mode activated" << endl;
		c.sendData("DEBUG");
		string msg = c.receive(32);
		if(msg.compare(0,6,"DEBUG1") == 0){
			cout << "Server: Debug active" << endl;
		}
		else{
			cout << "Server-ERROR: Debug failed!" << endl;
			sleep(10);
			return 0;
		}
	}
	else{
		cout << "Debug mode deactivated" << endl;
	}
	//*Choose Tactic
	cout << "Choose Tactic to test: " << endl;
	cout << "1: Longrun" << endl;
	cout << "2: Random" << endl;
	cout << "3: Hunt (random)" << endl;
	int tactic;
	cin >> tactic;
	switch (tactic) {
		case 1:
			cout << "Longrun chosen" << endl;
			break;
		case 2:
			cout << "Random chosen" << endl;
			break;
		case 3:
			cout << "Hunt (random) chosen" << endl;
			break;
		default:
			cout << "Invalid input, exiting" << endl;
			sleep(10);
			return 0;
	}

	//*Start Testing
	for(int n = 1; n<=Iterations; n++){
		triesArray[n] = 0;
		//Send newGame to server
		c.sendData("newGame");
		//Receive newGame from server
		string msg = c.receive(32);
		//cout << msg << endl;		 //Debug
		//Insert Strategy here;
		int currentRun = 0;
		switch(tactic) {
			case 1:
				currentRun = longrun();
				break;
			case 2:
				currentRun = randomRun();
				break;
			case 3:
				currentRun = HuntRandom();
				break;
			default:
				cout << "Invalid strategy, exiting" << endl;
				sleep(10);
				return 0;
		}
		triesArray[n] = currentRun;
		if(triesArray[n] == 0){
			cout << "WARNING: Ending program, because of empty strategy" << endl;
			sleep(10);
			return 0;
		}
		ExportData << n << ";" << currentRun << endl;
		cout << "#" << n << ":  " << to_string(triesArray[n]) << endl;
	}

	//*Calculate average
	//! Remove in final product... -> only for testing 
	int sum = 0;
	for (int i = 0; i < Iterations; i++) {
		sum += triesArray[i];
	}
	int average = sum / Iterations;
	cout << "-----------------------------------------"	<< endl;
	cout << "Average: " << average		<< endl;
	cout << "-----------------------------------------"	<< endl;
	//! ...till here

	//*Close file
	ExportData.close();
	if(ExportData.is_open() == false){
		cout << "Data Exported" << endl;
	}
	else{
		cout << "Error closing Datafile" << endl;
	}

	sleep(100);	//! Can be removed, only for testing so the console doesnt close immediately
	

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
			if(debug) sleep(1);
		}
	}	
	return -1;	//Wenn kein Game-Over erreicht wurde, wird -1 zurückgegeben, da dann ein Fehlversuch vorliegt.
}


int randomRun() {
	string msg;    // String, welcher zwischen Client und Server ausgetauscht wird
	int tries = 0;      // Anzahl der Versuche, bis das Spiel vorbei ist, wird zu Beginn auf 0 gesetzt
	set<int> usedCoordinates;   // Set, um bereits verwendete Koordinaten zu speichern

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 10);

	while (usedCoordinates.size() < 100) {
		int x = dis(gen);
		int y = dis(gen);
		int coordinate = x * 10 + y;

		if (usedCoordinates.count(coordinate) == 0) {
			usedCoordinates.insert(coordinate);
			tries++;    // Versuch wird bei jedem Schussversuch hochgezählt.
			msg = "COORD[" + to_string(x) + ";" + to_string(y) + "]";    // Nachricht wird zusammengesetzt
			// Der Server braucht dabei die Struktur: COORD[x;y], wobei x und y im Bereich [1,10]
			c.sendData(msg);    // Nachricht wird an Server gesendet
			msg = c.receive(32);    // Nachricht vom Server wird empfangen
			if (msg == "GAME OVER") {    // Wenn GAME OVER,
				return tries;                // dann wird die Anzahl der Versuche zurückgegeben
			}
			if(debug) sleep(1);
		}
	}
	return -1;    // Wenn kein Game-Over erreicht wurde, wird -1 zurückgegeben, da dann ein Fehlversuch vorliegt.
}

int HuntRandom() {
    string msg;    // String, welcher zwischen Client und Server ausgetauscht wird
	int tries = 0;      // Anzahl der Versuche, bis das Spiel vorbei ist, wird zu Beginn auf 0 gesetzt
	set<int> usedCoordinates;   // Set, um bereits verwendete Koordinaten zu speichern
	bool targetMode = false;
    int lastHitX = -1;
    int lastHitY = -1;
    int direction = 0; // 0 = up, 1 = right, 2 = down, 3 = left

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 10);

    while (usedCoordinates.size() < 100) {
        int x, y;

        if (targetMode) {
            // Try to sink the ship we've already hit
            switch (direction) {
                case 0: y = lastHitY - 1; x = lastHitX; break;
                case 1: x = lastHitX + 1; y = lastHitY; break;
                case 2: y = lastHitY + 1; x = lastHitX; break;
                case 3: x = lastHitX - 1; y = lastHitY; break;
            }
            direction = (direction + 1) % 4;
        } else {
            // Hunt for a new ship
            x = dis(gen);
            y = dis(gen);
        }

        int coordinate = x * 10 + y;

        if (usedCoordinates.count(coordinate) == 0) {
            usedCoordinates.insert(coordinate);
            tries++;
            msg = "COORD[" + std::to_string(x) + ";" + std::to_string(y) + "]";
            c.sendData(msg);
            msg = c.receive(32);

            if (msg == "HIT") {
                targetMode = true;
                lastHitX = x;
                lastHitY = y;
            } else if (msg == "MISS") {
                targetMode = false;
            } else if (msg == "GAME OVER") {
                return tries;
            }
			if(debug) sleep(1);
        }
    }
    return -1;
}
