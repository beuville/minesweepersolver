#ifndef GAMEINTERFACE_H_
#define GAMEINTERFACE_H_

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;


/* Definitions for Commands structure */
#define GETBOARD 0
#define PICKSQUARE 1

struct Game_specs{
	string difficulty;
	int Width;
	int Height;
	int Mines;
} board_specs;

/* Allows sendCommand function to accomodate multiple commands */
struct Commands{
	int cmd_num;
	int posx;
	int posy;
};

class GameInterface{
	public:
		Game_specs reportSpecs();
		int *** sendCommand(Commands cmd);
		float report();
		void initGame(string, int);
	private:
		Gameboard *board;
		int width;
		int height;
		int mines;
}
#endif 
