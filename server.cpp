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

#include <arpa/inet.h> // defines in_addr structure
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include <sys/socket.h> // for socket creation
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <unistd.h>    //contains various constants

#include "SIMPLESOCKET.H"

int main() {
  srand(time(nullptr));
  TCPserver srv(2022, 25);  // 2022 is the default port, 25 is the package size
  srv.run();                // run the server -> Method defined in SIMPLESOCKET.H and implemented in SIMPLESOCKET.CPP
}
