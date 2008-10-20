#ifndef PARSER_H_
#define PARSER_H_
#include <string>

#define SHORT_FIELD 24
#define LONG_FIELD 128
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
	char name[SHORT_FIELD];
	float value;
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
	char command[SHORT_FIELD];
	char action[SHORT_FIELD];
	char link[SHORT_FIELD];
	char syntax[LONG_FIELD];
};

/* pre_parse_array is a tool used to reduce lookups to other arrays while storing pre-parse relevant information in 
	single struct elements. During pre parse an array of these will be used to store information temporarily about each 
	line of program code.
*/
struct pre_parse_array{
	int action_index;
	char variables[LONG_FIELD];
};

class Parser{
	public:
		Parser(char ** code,int pcount,char * cfg_path);
		~Parser();
		void loadcfg(char * cfg_path);
		void preparse();
		void parse();

		int get_action(string line);		//returns action index of a given line
		
		int action_to_int(string action);	//returns int action values based on string conversion

		string get_variables(string line, string syntax); //returns variables from a given line of code based on syntax froma actions.cfg
		string get_value(string line,string var_type);	//function for returning specific variable values from coded get_varibales strings
	
		int get_next_link(string command, string links, int pcount);	//negotiates evaluate commands
		string eval_link(string command,string links); //helper function for get_next_link


		void disp_st();
	private:
		int p_count;		//holds the length of the code array
		int a_count;		//holds the lenght of the actions array
		int s_count;		//holds the lenght of the symbol table array
		string * program;	//program array
		symbol_table * st;	//symbol table array
		action * actions;	//action list array

};


#endif //PARSER_H_
