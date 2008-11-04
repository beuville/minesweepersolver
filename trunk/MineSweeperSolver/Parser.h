#ifndef PARSER_H_
#define PARSER_H_
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "GameInterface.h"

#define FALSE 0

#define DECLARE 100
#define UPDATE 101
#define EVALUATE 102 
#define MARKER 103
#define GOTO 104

#define DISPLAY 200

#define GAMESPEC 404

#define VALUE "<val>"
#define NAME "<nam>"
#define CONDITION "<cnd>"

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
	string name;
	double value;
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
struct action_struct{
	string command;
	string action;
	string link;
	string syntax;
};

/* pre_parse_array is a tool used to reduce lookups to other arrays while storing pre-parse relevant information in 
	single struct elements. During pre parse an array of these will be used to store information temporarily about each 
	line of program code.
*/
struct pre_parse_array{
	int action_index;
	int symbol_index;
	string variables;
};

class Parser{
	public:
		Parser(string * code, int pcount, string cfg_path);
		~Parser();
		
		void loadcfg(string cfg_path);
		void preparse();
			int get_action(string line);		//returns action index of a given line
		
			int action_to_int(string action);	//returns int action values based on string conversion

			string get_variables(string line, string syntax); //returns variables from a given line of code based on syntax froma actions.cfg
			string get_value(string line,string var_type);	//function for returning specific variable values from coded get_varibales strings
	
			int get_next_link(string command, string links, int pcount);	//negotiates evaluate commands
			string eval_link(string command,string links); //helper function for get_next_link

		double parse();
			string lookup_replace(string cmd);//lookup and replace
			double lookup_eval(string vars);	//lookup and evaluate
		void disp_at();
		void disp_st();
		void disp_ppa();
	private:
		int p_count;				//holds the length of the code array
		int a_count;				//holds the lenght of the actions array
		int s_count;				//holds the lenght of the symbol table array
		string * program;			//program array
		symbol_table * st;			//symbol table array
		action_struct * actions;	//action list array
		pre_parse_array * ppa;		//meta-data for program code

		GameInterface gi;

};

//float to string converter used in lookup_replace
string float_to_a(const double value)
{
    stringstream converter;
    string  str;

    converter << value;
    converter >> str;
    return str;
}

Parser::Parser(string * code, int pcount, string cfg_path){
	p_count = pcount;	//assign length of code to private variable
	//load up the program array
	program = new string[p_count];
	//line by line
	for(int i = 0;i<p_count;i++){
		program[i]=code[i];
	}
	loadcfg(cfg_path);		//load configuration file into memory
	//cout << "configuration loaded\n";
	gi.initGame("easy",-1);	//initialize game board
	//cout << "game initialized\n";
	preparse();				//load symbol table, and ppa
	//cout << "preparse completed\n";
}
/*Destructor
*/
Parser::~Parser(){
	delete[] actions;
	delete[] st;
	delete[] ppa;
}

/*loadcfg
* open the config file and read the actions into the actions array
* ignore lines starting with #
* also dynamically allocates actions array
* returns 0 on success, error codes for file not found etc
*/
void Parser::loadcfg(string cfg_path){
	//initialize 
	ifstream cfg_fp;
	char line_in[256];
	char * next_token;
	int line_count = 0;

	//open config file
	cfg_fp.open(cfg_path.c_str(),ifstream::in);

	//count lines that are not commented (don't begin with #)
	while(cfg_fp.getline(line_in,256)){
		if(line_in[0] != '#'){
			line_count++;
		}
	}
	//rewind config file
	cfg_fp.clear();
	cfg_fp.seekg(0,ios::beg);
	//allocate actions memory
	actions = new action_struct[line_count];
	a_count = line_count; //store the actions count in a private variable
	//fill actions table
	line_count=0;
	while(cfg_fp.getline(line_in,256)){
		if(line_in[0] != '#'){
			actions[line_count].command=strtok_s(line_in,"\t",&next_token);
			actions[line_count].action=strtok_s(NULL,"\t",&next_token);
			actions[line_count].link=strtok_s(NULL,"\t",&next_token);
			actions[line_count].syntax=strtok_s(NULL,"\t",&next_token);
			line_count++;
		}
	}
	//cleanup
	cfg_fp.close();
}

/*preparse
* load the symbol table according to actions array
*/
void Parser::preparse(){
	int symbol_count = 0;	//counter for lines in the symbol table
	ppa = new pre_parse_array[p_count]; //allocate meta-data table entries for each line of code

	//go line by line through program code to determine the size of the symbol table
	//all declare, evaluate, marker and goto commands will need entries in the symbol
	//table
	for(int i=0;i<p_count;i++){
		//load ppa entry
		ppa[i].action_index = get_action(program[i]);
		//dont get variables for game specific commands
		if(ppa[i].action_index != 0){
			ppa[i].variables = get_variables(program[i],actions[ppa[i].action_index].syntax);
		}
		//switch action value, because the first step will be to count space needed in
		//	the symbol table, each action that requires room in the symbol table 
		//	(declare,evaluate,marker,goto)
		switch(action_to_int(actions[ppa[i].action_index].action)){
			case DECLARE: 
			case EVALUATE:
			case MARKER:
			case GOTO: 
			case GAMESPEC:	symbol_count++; break;
		}

	}
	symbol_count+=1;//make room for the gamespec return variable
	
	//with symbol_count accurate, time to allocate the symbol table array
	st = new symbol_table[symbol_count];
	s_count = symbol_count;

	//populte the symbol table based on actions. for more detail view the programming language definition.	
	//start by looping the program array once again, we need a value to hold the next open location in symbol
	//	table, we will re-use symbol_count now that s_count permantly holds its value.
	
	//first create a variable at location 0 that will hold the return value from gamspec commands
	st[0].name = "GSRETURN";
	st[0].value = 0;
	st[0].count = -1;
	st[0].location = 0;

	symbol_count = 1;
	//cmd_count will serve to act as the 'count' variable for symbol table entries
	//opening and closing statements for loop and conditional statements will have the same count value
	//while symbol_count is updated after every entry in the symbol table, cmd_count will only be updated
	//after a command group is completely filled (i.e when the end if for an if structure is logged)
	int cmd_count =0;
	int next_link;
	//This is the main loop responsible for filling the symbol table
	for(int i=0;i<p_count;i++){
		//-after each sucessfull entry in the symbol table symbol_count needs to be incremented
		//-for detailed information about preparser actions, consult the programming language definition
		switch(action_to_int(actions[ppa[i].action_index].action)){
			case DECLARE: 
				st[symbol_count].name = get_value(ppa[i].variables,NAME);//load name
				st[symbol_count].value = atof(get_value(ppa[i].variables,VALUE).c_str());//load value
				st[symbol_count].location = i;		//load location
				st[symbol_count].count = cmd_count; //load count
				ppa[i].symbol_index = symbol_count;
				cmd_count++;
				symbol_count++;
				break;
			//evaluate commands (if, elseif, and for) will have symbol table entries based on their link properties
			case EVALUATE:
				next_link = i;
				//resolve the links
				do{
					ppa[next_link].symbol_index = symbol_count;
					st[symbol_count].name = actions[ppa[next_link].action_index].command;//load name
					st[symbol_count].location = next_link;	//load location
					next_link = get_next_link(actions[ppa[next_link].action_index].command,actions[ppa[next_link].action_index].link,next_link);
					st[symbol_count].value = next_link;	//load value
					st[symbol_count].count = cmd_count;	//load count
					symbol_count++;
				}while(actions[ppa[next_link].action_index].link.compare("nill")!=0);
				//closing statement entries in the symbol table (eif, efor, etc)
				st[symbol_count].name = actions[ppa[next_link].action_index].command;//load name
				st[symbol_count].location = next_link;	//load location
				st[symbol_count].value = i;				//back to the top for the value
				st[symbol_count].count = cmd_count;		//load count
				ppa[next_link].symbol_index = symbol_count;
				symbol_count++;
				cmd_count++;
				break;
			case GAMESPEC:
				st[symbol_count].name = "gamespec";
				st[symbol_count].location = i;
				st[symbol_count].value = i;
				st[symbol_count].count = -1;
				ppa[i].symbol_index = symbol_count;
				symbol_count++;
				break;
			case UPDATE:
				ppa[i].symbol_index = -1;
				break;
		}
	}
}
/* get_action
* returns the index of the action associated with a given line
*/
int Parser::get_action(string line){
	//check the actions table line by line to compare syntax with the parameter line
	//if a syntax matches return the appropriate action. 
	for(int i =0;i<a_count;i++){
		//due to the fact that the programming language definition demands that all syntax be 
		//	prefixed by the command name, identifying the action can be accomplised with simple 
		//  string parsing
		//if the line has an occurance of the name of a function defined by actions.cfg then
		//	we can return the corresponding action index.
		if(line.find(actions[i].command,0) == 0){
			return i;
		}
	}
	//no syntax matched, we will return a value that will associate this line with game-specific commands
	return 0;
}

/* get_variables
*	returns a coded string containing all variables from a given line of code based on syntax from the actions.cfg fing
*	strings will be returned in the format <var>value,<var2>value2,... example: <nam>var1,<val>100
*/
string Parser::get_variables(string line, string syntax){
	string ret = "";
	//as long as there are more variables to be found...
	while(syntax.find_first_of('<') != string::npos){	
		//remove syntactical prefixes
		line=line.substr(syntax.find_first_of('<'));
		syntax=syntax.substr(syntax.find_first_of('<'));
		//append next variable type
		ret = ret + syntax.substr(0,5);
		//remove variable tag to get to next separator
		syntax=syntax.substr(5);
		//append variable value and a separator (',')
		ret = ret + line.substr(0,line.find_first_of(syntax.substr(0,syntax.find_first_of('<')))) + ',';
		//remove variable up to next separtor from line
		line = line.substr(line.substr(0,line.find_first_of(syntax.substr(0,syntax.find_first_of('<')))).length());
	}
	return ret;
}

/* get_value
* returns a value based on a given variable type (<nam><val><cnd>)
*/
//get_nam
string Parser::get_value(string line, string var_type){
	int start = line.find(var_type) + var_type.length();
	string s1 = line.substr(start,line.find_first_of(',',start)-start);
	//trim then return
	s1=s1.substr(s1.find_first_not_of(' '),s1.find_last_not_of(' ')+1);
	return s1;
}

/* get_next_link
*	returns the line number of the next link related to a given command. starting at pcount for example
*	command: if	links: elseif|else|endif
*	will search for the next elseif,else while negotiating nested if commands.
*/
int Parser::get_next_link(string command, string links, int pcount){
	int nest_count=0;
	//loop doesnt check to make sure pcount stays within the program array max index, but a well formed
	//program should never raise such an exception
	while(true){
		//start with the line after the give command, contines as incrementor
		pcount +=1;
		//if we come accros another command of the same type, we have found a nested conditoin
		if(actions[get_action(program[pcount])].command.compare(command.c_str())==0){
			nest_count +=1;
		}
		//found a match!
		if(eval_link(program[pcount],links)!= "")
		{		
			//if nest count is not zero we have jumped into another struture's nesting
			if(nest_count != 0){
				//if the current line of code we are examining has no link value(link==nill),we must decrement the nest_count 
				//this can be determined with a simple lookup in the actions table
				for(int i = 0;i<a_count;i++){
				//found the command
					if(program[pcount].compare(actions[i].command)==0){
						if(actions[i].link.compare("nill")==0){			
							nest_count -=1;
						}
					}
				}
			}
			//found our match
			else{
				return pcount;
			}
		}
	}
}

/* eval_link
*	returns the matched link if one occurs in string, essentiall a helper function for get_next_link
*/
string Parser::eval_link(string command,string links){
	while(links.length() != 0)
	{
		if(links.find_first_of('|') == string::npos){
			if(links.compare(command) == 0){
				return command;
			}
			return "";
		}
		else{
			//cout <<links.substr(0,links.find_first_of('|')) << "\n";
			if(command.compare(links.substr(0,links.find_first_of('|'))) == 0){
				return command;
			}
			links.erase(0,links.find_first_of('|') + 1);
		}
	}
	return "";
}

//factoring out of a commonly used specific string to integer converter
int Parser::action_to_int(string action){
	if(action.compare("declare")==0){return DECLARE;}
	else if(action.compare("evaluate")==0){return EVALUATE;}
	else if(action.compare("update")==0){return UPDATE;}
	else if(action.compare("marker")==0){return MARKER;}
	else if(action.compare("goto")==0){return GOTO;}
	else if(action.compare("gamespec")==0){return GAMESPEC;}
	else if(action.compare("display")==0){return DISPLAY;}
	else{return -1;}
}

/*parse
* parse the array of code lines
*/
double Parser::parse(){
	int pc = 0;//program counter
	string s1;
	string s2;
	//go the program line by line, use the ppa in conjuntion with actions table and symbol 
	//table to determine the necessary action
	while(pc < p_count){	//as long as pc has not exceeded the total length of the program, keep executing
		//switch the action associated with the current line of code pointed to by pc
		switch(action_to_int(actions[ppa[pc].action_index].action)){
			//for specific information on what actions are taken in the parseing stage see 
			//programming language definition
			case UPDATE:
				//replace variable names in 'value' field with lookups from the symbol table
				//lookup update variable name
				s1 = get_value(ppa[pc].variables,NAME);//value name
				for(int i=0;i<s_count;i++){
					//if the name field from the program line and the symbol table name are the same					
					s2 = string(st[i].name);//symbol table name
					if(s1.compare(s2)==0){
						//update the value in the symbol table
						st[i].value = lookup_eval(get_value(ppa[pc].variables,VALUE));
						break;
					}
				}
				pc+=1;
				break;
			case GOTO:
				//move the pc to the line number indicated by the symbol table
				pc=(int)st[ppa[pc].symbol_index].value;
				break;
			case EVALUATE:
				//lookup_eval the condition, if it is false (0), move pc to one line past the link value
				if(lookup_eval(get_value(ppa[pc].variables,CONDITION)) == FALSE){
					pc = (int)st[ppa[pc].symbol_index].value + 1;
				}
				else{		//if it is true increment pc
					pc+=1;
				}
				break;
			case GAMESPEC:
				//return value from sendCommand is stored at location 0(GSRETURN) in the symbol table
				st[0].value = gi.sendCommand(lookup_replace(program[pc])); 
				//after every game specific command we will evaluate the status of the game
				if(gi.report_status()==false){
					//if the previously issued command ended the game, we will return the completion
					//percentage of the board, otherwise, play on
					return gi.report();
				}
				pc+=1;
				break;
			case DISPLAY:
				if(actions[get_action(program[pc])].command.compare("disp")==0){
					s1 = get_value(ppa[pc].variables,NAME);//value name
					for(int i=0;i<s_count;i++){
						//if the name field from the program line and the symbol table name are the same					
						s2 = string(st[i].name);//symbol table name
						if(s1.compare(s2)==0){
							//output the value
							cout << "msso: " << st[i].value<<"\n";
							break;
						}
					}
				}
				else if(actions[get_action(program[pc])].command.compare("write")==0){
					cout << "msso: " << get_value(ppa[pc].variables,NAME)<<"\n";
				}
				pc+=1;
				break;
			default:
				pc+=1;
		}
	}
	//if the whole program loop falls through, return the status of board
	return gi.report();
}
/* lookup_replace
*	return a string containing values for any variables found in cmd string, replaced from
*	symbol table (used specifically in handling GAMESPEC commands)
*	currently this function assumes variables will appear only onces in a given string
*	furthermore, this function will not negotiate variable operations within the string
*		i.e picsq x+1,y+2	however, it is possible to use lookup_eval to accomplish this, just not now
*/
string Parser::lookup_replace(string cmd){
	//loop through the symbol table
	for(int i =0;i<s_count;i++){
		string::size_type loc = cmd.find(st[i].name);
		//found a match
		if(loc != string::npos){
			//erase variable name
			cmd.erase(loc,st[i].name.length());
			//insert new value
			cmd.insert(loc,float_to_a(st[i].value));
		}
	}
	//once all replacements have been made, return the string.
	return cmd;
}
/* lookup_eval
*	lookup the values of variables in the string vars, then evaluate the string returing an integer value
*/
double Parser::lookup_eval(string vars){
	char op;
	string s1;
	string s2;
	double var1;
	double var2;
	for(unsigned int i = 0;i < vars.length();i++){
		switch(vars[i]){
			//find the location of operator
			case '*':
			case '/':
			case '+':
			case '-':
			case '=':
			case '<':
			case '>':
			case '#':
			case '$':
			case '%':
			case '!':
			//store the operator
			op = vars[i];
			//parse,lookup the operands
			s1 = vars.substr(0,i);
			s2 = vars.substr(i+1,vars.length());
			//trim strings
			s1=s1.substr(s1.find_first_not_of(' '),s1.find_last_not_of(' ')+1);
			s2=s2.substr(s2.find_first_not_of(' '),s2.find_last_not_of(' ')+1);
			//initialize variables
			var1=atof(s1.c_str());
			var2=atof(s2.c_str());
			//lookup variable values
			for(int j =0;j<s_count;j++){
				if(s1.compare(st[j].name)==0){
					var1 = st[j].value;
				}
			}
			for(int j =0;j<s_count;j++){
				if(s2.compare(st[j].name)==0){
					var2 = st[j].value;
				}
			}
			//evaluate and return
			switch(op){
				case '*':
					return var1 * var2;
					break;
				case '/':
					return var1 / var2;
					break;
				case '+':
					return var1 + var2;
					break;
				case '-':
					return var1 - var2;
					break;
				case '=':
					return var1 == var2;
					break;
				case '<':
					return var1 < var2;
					break;
				case '>':
					return var1 > var2;
					break;
				case '#':
					return var1 <= var2;
					break;
				case '$':
					return var1 >= var2;
					break;
				case '%':
					return (int)var1 % (int)var2;
					break;
				case '!':
					return var1 != var2;
					break;
			}
		}
	}
	//if the loop falls through, there were no operators, lookup the value
	//lookup variable values
	for(int j =0;j<s_count;j++){
		if(vars.compare(st[j].name)==0){
			return st[j].value;
		}
	}
	//or return the value
	return atoi(vars.c_str());
}

//crude display functions
void Parser:: disp_at(){
	cout << "cmd\tact\tlnk\tstx\n";
	for(int i=0;i<a_count;i++){
		cout <<actions[i].command<<"\t"<<actions[i].action<<"\t"<<actions[i].link<<"\t"<<actions[i].syntax<<"\n";
	}
	cout <<"\n";
}

void Parser:: disp_st(){
	cout << "name\tvalu\tloca\tcoun\n";
	for(int i=0;i<s_count;i++){
		cout<<st[i].name<<"\t"<<st[i].value<<"\t"<<st[i].location<<"\t"<<st[i].count<<"\n";
	}
	cout <<"\n";
}

void Parser:: disp_ppa(){
	for(int i=0;i<p_count;i++){
		cout <<ppa[i].action_index<<"\t"<<ppa[i].symbol_index<<"\t"<<program[i]<<"\t"<<ppa[i].variables<<"\n";
	}
	cout <<"\n";
}
#endif //PARSER_H_
