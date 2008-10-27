#include "Gameboard.h"
#include "GameInterface.h"

;//Compile errors in VS9 without ";"

using namespace std;

Game_specs GameInterface::reportSpecs(){
	return board_specs;
}

/* Send game commands to the gameboard
 * Allowed commands are:
 *		GETBOARD - returns integer array of current board values
 *		PICKSQUARE - picks the square at x,y coordinate
 */
int *** GameInterface::sendCommand(Commands cmd){
	if(cmd.cmd_num == GETBOARD){
		return board->getBoard();
	}else if(cmd.cmd_num == PICKSQUARE){
		board->pickSquare(cmd.posx, cmd.posy);
	}
	return NULL;
}

/* The percentage of completion on this board
   return - board completion percentage
 */
float GameInterface::report(){
	return board->percentComplete();
}

/* Initializes a gameboard of mine sweeper
 * difficulty - the game difficulty
 * seed - the random seed value
 */
void GameInterface::initGame(string difficulty="easy", int seed=-1){
	if(difficulty.compare("easy")){
		width = 9;
		height = 9;
		mines = 10;
	}else if(difficulty.compare("intermediate")){
		width = 16;
		height = 16;
		mines = 40;
	}else if(difficulty.compare("hard")){
		width = 30;
		height = 16;
		mines = 99;
	}else{
		perror("Unknown difficulty setting, shutting down");
		exit(1);
	}

	board = new Gameboard(width, height, mines, seed);
	board_specs.Height = height;
	board_specs.Width = width;
	board_specs.Mines = mines;
	board_specs.difficulty = difficulty;
}

