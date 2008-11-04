#include "Gameboard.h"
#include "GameInterface.h"

using namespace std;

Game_specs GameInterface::reportSpecs(){
	return board_specs;
}

/* Send game commands to the gameboard
 * commands are 5 characters long
 * Allowed commands are:
 *		getsq x,y - returns integer value of square x,y
 *		picsq x,y - picks the square at x,y coordinate
 */
double GameInterface::sendCommand(string cmd){
	//cout << "GI:"<<cmd<<":\n";
	string vars;
	string command = cmd.substr(0,5);
	int x,y;
	//getsq function
	if(command.compare("getsq")==0){
		vars = cmd.substr(6,cmd.length());
		x = atoi(vars.substr(0,vars.find_first_of(',')).c_str());
		y = atoi(vars.substr(vars.find_first_of(',')+1,vars.length()).c_str());
		return board->getCoordinate(x,y,'p');
	}
	else if(command.compare("picsq")==0){
		vars = cmd.substr(7,cmd.length());
		x = atoi(vars.substr(0,vars.find_first_of(',')).c_str());
		y = atoi(vars.substr(vars.find_first_of(',')+1,vars.length()).c_str());
		board->pickSquare(x,y);
		//return the value of the square we just picked
		return board->getCoordinate(x,y,'p');
	}
	else{
		return -1;
	}
}

/* The percentage of completion on this board
   return - board completion percentage
 */
double GameInterface::report(){
	return board->percentComplete();
}

/* Returns a true value if the current game is still being played (no win or loss conditions met)
	returns false if the game has concluded
*/
bool GameInterface::report_status(){
	return board->progress();
}

/* Initializes a gameboard of mine sweeper
 * difficulty - the game difficulty
 * seed - the random seed value
 */
void GameInterface::initGame(string difficulty="easy", int seed=-1){
	if(difficulty.compare("easy")==0){
		width = 9;
		height = 9;
		mines = 10;
	}else if(difficulty.compare("intermediate")==0){
		width = 16;
		height = 16;
		mines = 40;
	}else if(difficulty.compare("hard")==0){
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

