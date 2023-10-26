//============================================================================
// Name        : INF3_Prak.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
#include <string>

#include <cstdio>  /* printf, NULL */
#include <cstdlib> /* srand, rand */
#include <ctime>   /* time */

#include <unistd.h>

using namespace std;

int main() {
  /*srand(time(nullptr));
  TASK3::demoTask3();*/
  system("gnome-terminal -- ./server");
  system("gnome-terminal -- ./client");
  cout << "You Won!";
  return 0;
}
