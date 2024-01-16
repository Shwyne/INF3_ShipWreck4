/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */
#include <cerrno> //It defines macros for reporting and retrieving error conditions through error codes
#include <cstdio> // standard input and output library
#include <cstdlib> // this includes functions regarding memory allocation
#include <cstring> // contains string functions
#include <ctime>   //contains various functions for manipulating date and time
#include <iostream>

#include <arpa/inet.h> // defines in_addr structure
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include <sys/socket.h> // for socket creation
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <unistd.h>    //contains various constants

#include "SIMPLESOCKET.H"
#include "TASK3.H"

class TASK3server : public TCPserver {
    private:
        TASK3::World *w;    //
    public:
        TASK3server(int port, int packageSize) : TCPserver(port, packageSize) {
          w = new TASK3::World();
        }
        string myResponse(string input){
          
          if(input.compare(0,7,"newGame") == 0){
            delete w;
            w = new TASK3::World();
            return string("newGame");
          }
          else if(input.compare(0,6,"COORD[") == 0){
            int x, y;
            TASK3::ShootResult e;
            string sx = input.substr(6,1);
            string sy = input.substr(8,1);
            sscanf(sx.c_str(), "%d", &x);
            sscanf(sy.c_str(), "%d", &y);
            e = w->shoot(x,y);
            w->printBoard();
            return ShootResultToString(e);
          }
          else{
            return string("ERROR: UNKNOWN COMMAND");
          }
        }
        string ShootResultToString(TASK3::ShootResult e){
          switch(e){
            case TASK3::WATER:
              return string("0");
            case TASK3::SHIP_HIT:
              return string("1");
            case TASK3::SHIP_DESTROYED:
              return string("2");
            case TASK3::GAME_OVER:
              return string("GAME OVER");
            default:
              return string("ERROR: UNKNOWN SHOOTRESULT");
          }
        }
};

int main() {
  std::cout << "Server is running..." << std::endl;
  srand(time(nullptr));
  TASK3server srv(2022, 25);  // 2022 is the default port, 25 is the package size
  srv.run();                // run the server -> Method defined in SIMPLESOCKET.H and implemented in SIMPLESOCKET.CPP
}

// if (compare string) -> delete w; w = new TASK3::World();
//else return string("ERROR: UNKNOWN COMMAND");
/*Schusskoordinate empfangen -> if(input.compare(0,6,"COORD[]")){
  int x,y;
  TASK3::ShootResult e;
  string -> x,y
  e = w->shoot(x,y);
  return msg(e);
}

*/