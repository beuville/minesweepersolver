#ifndef GAMEINTERFACE_H_
#define GAMEINTERFACE_H_

#include <stdlib.h>
#include <iostream>
#include <string>
#include "Gameboard.h"

using namespace std;


/* Definitions for Commands structure */
#define GETBOARD 0
#define PICKSQUARE 1

struct Game_specs{
	string difficulty;
	int Width;
	int Height;
	int Mines;
};


/* Allows sendCommand function to accomodate multiple commands */
struct Commands{
	int cmd_num;
	int posx;
	int posy;
};

class GameInterface{
	public:
		Game_specs reportSpecs();
		double sendCommand(string cmd);
		double report();
		bool report_status();
		void initGame(string, int);
	private:
		Gameboard *board;
		int width;
		int height;
		int mines;
		Game_specs board_specs;
};
#endif 
