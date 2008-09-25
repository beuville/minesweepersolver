#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"
using namespace std;

/*Constructor 
* string cfg: location of the config file defaulted to actions.cfg
*/
Parser::Parser(char ** code){
}
/*Destructor
*/
Parser::~Parser(){
	delete[] actions;
}

/*loadcfg
* open the config file and read the actions into the actions array
* ignore lines starting with #
* also dynamically allocates actions array
* returns 0 on success, error codes for file not found etc
*/
void Parser::loadcfg(char * cfg_path){
	ifstream cfg_fp;
	char line_in[256];
	int line_count = 0;

	cfg_fp.open(cfg_path,ifstream::in);
	
	//count lines that are not commented (don't begin with #)
	while(cfg_fp.getline(line_in,256)){
		if(line_in[0] != '#'){
			line_count++;
		}
	}
	//rewind
	cfg_fp.clear();
	cfg_fp.seekg(0,ios::beg);
	//allocate actions memory
	actions = new action[line_count];
	
	//fill actions table
	line_count=0;
	while(cfg_fp.getline(line_in,256)){
		if(line_in[0] != '#'){
			actions[line_count].command = strtok(line_in,"\t");
			actions[line_count].action = strtok(NULL,"\t");
			actions[line_count].link = strtok(NULL,"\t");
			actions[line_count].syntax = strtok(NULL,"\t");
			line_count++;
		}
	}

	cfg_fp.close();
}

/*preparse
* load the symbol table according to actions array
*/
void Parser::preparse(){
}

/*parse
* parse the array of code lines
*/
void Parser::parse(){
}


int main(){
	char * program[100] = {"int var1=100","int var2=200","float var3=10.1"};
	Parser p(program);
	p.loadcfg("actions.cfg");
}
