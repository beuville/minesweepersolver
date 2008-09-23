#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Parser.h"
using namespace std;

/*Constructor 
* string cfg: location of the config file defaulted to actions.cfg
*/
Parser::Parser(){
}
/*Destructor
*/
Parser::~Parser(){
}

/*loadcfg
* open the config file and read the actions into the actions array
* ignore lines starting with #
* also dynamically allocates actions array
* returns 0 on success, error codes for file not found etc
*/
int Parser::loadcfg(){
	return 0;
}

/*preparse
* load the symbol table according to actions array
*/
int Parser::preparse(){
	return 0;
}

/*parse
* parse the array of code lines
*/
int Parser:parse(){
	return 0;
}


int main(){
Parser p;
p.loadcfg();
}
