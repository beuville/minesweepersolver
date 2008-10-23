#ifndef GAMEINTERFACE_H_
#define GAMEINTERFACE_H_

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

struct report_specs{
	float percent_complete;
	string difficulty;
};

class GameInterface{
	public:
		float reportSpecs();
		int sendCommand();
		report_specs report();
		int initGame();
}
#endif 
