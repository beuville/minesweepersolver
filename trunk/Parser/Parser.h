#ifndef PARSER_H_
#define PARSER_H_
#include <string>
using namespace std;

/* symbol_table is a single entry in the symbol table array containing information about
	variable declarations, loops, and control statments

	these will be the commands found in the symbol table. Due to the fact that commands will
	be read in from a config file however, they are subject to change.

	name:	value:												
	int		int value
	float	float value
	
	if		next else line# or eif line# if no else present
	eif		if line#
	elseif	next else line# or eif if last else statement
	else	next else line# or eif if last else statement

	for		efor line#	
	efor	for line#
	do		edo line#
	edo		do line#
	while	ewhile line#
	ewhile	while line#

	location:
	each command in symbol table will have the array index that it can be found at in parsed programs source

	count:
	each variable, and each group of if/else/eif, for/efor, do/edo, while/ewhile will have a unique count to 
	identify themselves and group together control/loop structures.
*/
struct symbol_table{
	char name[16];
	int value;
	int location;
	int count;
};

/* actions are individual lines in the actions array. They are read in from a config file and tell the parser what
	action to take when a given command is read in. This allows the parseing language to be defined somewhat
	dynamically from said config file. This also focuses the language definition into one location so later
	communications with the main program will be synchronized by reading the same file. 

	sytnax will be used in conjunction with regular expression matching to identify commands. 
	See actions.cfg
*/
struct action{
	char * command;
	char * action;
	char * link;
	char * syntax;
};

class Parser{
	public:
		Parser(char ** code);
		~Parser();
		void loadcfg(char * cfg_path);
		void preparse();
		void parse();
	private:
		string * program;	//program array
		symbol_table * st;	//symbol table array
		action * actions;	//action list array

};


#endif //PARSER_H_
