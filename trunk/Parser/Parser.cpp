#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"

#define DECLARE 100
#define UPDATE 101
#define EVALUATE 102 
#define MARKER 103
#define GOTO 104

#define VALUE "<val>"
#define NAME "<nam>"
#define CONDITION "<cnd>"

using namespace std;

/*Constructor 
* string cfg: location of the config file defaulted to actions.cfg
*/
Parser::Parser(char ** code, int pcount, char * cfg_path){
	p_count = pcount;	//assign length of code to private variable
	//load up the program array
	program = new string[p_count];
	//line by line
	for(int i = 0;i<p_count;i++){
		program[i]=code[i];
	}
	
	//load configurate, preparse then parse
	loadcfg(cfg_path);
	preparse();
	//parse();
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
	//initialize 
	ifstream cfg_fp;
	char line_in[256];
	int line_count = 0;
	
	//open config file
	cfg_fp.open(cfg_path,ifstream::in);
	
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
	actions = new action[line_count];
	a_count = line_count; //store the actions count in a private variable
	//fill actions table
	line_count=0;
	while(cfg_fp.getline(line_in,256)){
		if(line_in[0] != '#'){
			strcpy(actions[line_count].command,strtok(line_in,"\t"));
			strcpy(actions[line_count].action,strtok(NULL,"\t"));
			strcpy(actions[line_count].link,strtok(NULL,"\t"));
			strcpy(actions[line_count].syntax,strtok(NULL,"\t"));
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
	pre_parse_array * ppa = new pre_parse_array[p_count]; //allocate meta-data table entries for each line of code

	//go line by line through program code to determine the size of the symbol table
	//all declare, evaluate, marker and goto commands will need entries in the symbol
	//	table
	for(int i=0;i<p_count;i++){
		//load ppa entry
		program[i].copy(ppa[i].line, sizeof ppa[i].line);
		ppa[i].action = get_action(program[i]);
		strcpy(ppa[i].syntax,get_syntax(program[i]));
		strcpy(ppa[i].command,get_command(program[i]));
		strcpy(ppa[i].variables,get_variables(ppa[i].line,ppa[i].syntax).c_str());
		//switch action value, because the first step will be to count space needed in
		//	the symbol table, each action that requires room in the symbol table 
		//	(declare,evaluate,marker,goto)
		switch(ppa[i].action){
			case DECLARE: 
			case EVALUATE:
			case MARKER:
			case GOTO: symbol_count++; break;
		}

	}
	
	//with symbol_count accurate, time to allocate the symbol table array
	st = new symbol_table[symbol_count];
	s_count = symbol_count;

	//populte the symbol table based on actions. for more detail view the programming language definition.	
	//start by looping the program array once again, we need a value to hold the next open location in symbol
	//	table, we will re-use symbol_count now that s_count permantly holds its value.
	symbol_count = 0;
	//another variable is needed to adress the symbol table count
	//cmd_count will serve to act as the 'count' variable for symbol table entries
	//opening and closing statements for loop and conditional statements will have the same count value
	//while symbol_count is updated after every entry in the symbol table, cmd_count will only be updated
	//after a command group is completely filled (i.e when the end if for an if structure is logged)
	int cmd_count =0;
	//another variable is needed to adress nested conditionals and loops. 
	//nest_count will be incremented when a new conditional or loop is identified, then the preparser will read ahead
	//until it resolves nest_count when it finds the corresponding end statement for the structure and is thus decremented
	//back to zero. This is simplified somewhat as there are unique end statements for all structures capable of being nested
	int nest_count = 0;

	for(int i=0;i<p_count;i++){
		//-after each sucessfull entry in the symbol table symbol_count needs to be incremented
		//-for detailed information about preparser actions, consult the programming language definition
		switch(ppa[i].action){
			case DECLARE: 
				strcpy(st[symbol_count].name, get_value(ppa[i].variables,NAME).c_str());//load name
				st[symbol_count].value = atof(get_value(ppa[i].variables,VALUE).c_str());//load value
				st[symbol_count].location = i;//load location
				st[symbol_count].count = cmd_count;
				cmd_count++;
				symbol_count++;
				break;
			case EVALUATE:
				symbol_count++;
				break;
			case MARKER:
				strcpy(st[symbol_count].name,ppa[i].command);//load name
				st[symbol_count].value = i;//load value
				st[symbol_count].location = i;//load location
				st[symbol_count].count = cmd_count;
				cmd_count++;
				symbol_count++;
				break;
			case GOTO: 
				symbol_count++; 
				break;
			//in this case the default represents commands that don't require space in the symbol table
			//(game specific commands and updates) and thus should not increment symbol_count
			default: cout<<"defualt\n";
		}
	}
}
/* get_action
* function used to determine which action from the action.cfg file a given line uses based on 
* the lines syntax compared to the syntaxs found in the same config file.
*/
int Parser::get_action(string line){
	//check the actions table line by line to compare syntax with the parameter line
	//if a syntax matches return the appropriate action. 
	for(int i =0;i<a_count;i++){
		//due to the fact that the programming language definition demands that all syntax be 
		//	prefixed by the command name, identifying the action can be accomplised with simple 
		//  string parsing
		//if the line has an occurance of the name of a function defined by actions.cfg then
		//	we can return the corresponding action.
		if(line.find(actions[i].command,0) == 0){
			return(action_to_int(actions[i].action));
		}
	}
}
/* get_syntax
*/
/* get_command
* both these functions follow the same format as get_action minus the string conversion
*/
char * Parser::get_syntax(string line){
	for(int i =0;i<a_count;i++){
		if(line.find(actions[i].command,0) == 0){
			return actions[i].syntax;
		}
	}
	//if command is not recogniced returns "nocmd"
	return "nocmd";
}
char * Parser::get_command(string line){
	for(int i =0;i<a_count;i++){
		if(line.find(actions[i].command,0) == 0){
			return actions[i].command;
		}
	}
	return "nocmd";
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
	return line.substr(start,line.find_first_of(',',start)-start);
}


//factoring out of a commonly used specific string to integer converter
int Parser::action_to_int(string action){
	if(action.compare("declare")==0){return DECLARE;}
	else if(action.compare("evaluate")==0){return EVALUATE;}
	else if(action.compare("update")==0){return UPDATE;}
	else if(action.compare("marker")==0){return MARKER;}
	else if(action.compare("goto")==0){return GOTO;}
}

/*parse
* parse the array of code lines
*/
void Parser::parse(){
}


void Parser:: disp_st(){
	cout << "name\tvalu\tloca\tcoun\n";
	for(int i=0;i<s_count;i++){
		cout<<st[i].name<<"\t"<<st[i].value<<"\t"<<st[i].location<<"\t"<<st[i].count<<"\n";
	}
}
int main(){
	char * program[100] = {"int var1=100","int var2=200","float var3=10.1","eif","else"};
	Parser p(program,5,"actions.cfg");
	//cout << p.get_variables("int var1=100","int <nam>=<val>") << "\n";
	//cout << p.get_variables("ewhile","ewhile") << "\n";
	//cout <<p.get_value("<nam>var1,<val>200,","<nam>")<<"\n";
	//cout <<p.get_value("<nam>var1,<val>200,","<val>")<<"\n";
	p.disp_st();
	return 0;
}
