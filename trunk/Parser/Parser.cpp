#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"

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

/*Constructor 
* string cfg: location of the config file defaulted to actions.cfg
*/
Parser::Parser(string * code, int pcount, char * cfg_path){
	p_count = pcount;	//assign length of code to private variable
	//load up the program array
	program = new string[p_count];
	//line by line
	for(int i = 0;i<p_count;i++){
		program[i]=code[i];
	}
	//load configure, preparse then parse
	loadcfg(cfg_path);
	preparse();
	parse();
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
	actions = new action_struct[line_count];
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
	ppa = new pre_parse_array[p_count]; //allocate meta-data table entries for each line of code

	//go line by line through program code to determine the size of the symbol table
	//all declare, evaluate, marker and goto commands will need entries in the symbol
	//table
	for(int i=0;i<p_count;i++){
		//load ppa entry
		ppa[i].action_index = get_action(program[i]);
		//dont get variables for game specific commands
		if(ppa[i].action_index != 0){
			strcpy(ppa[i].variables,get_variables(program[i],actions[ppa[i].action_index].syntax).c_str());
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
	
	//with symbol_count accurate, time to allocate the symbol table array
	st = new symbol_table[symbol_count];
	s_count = symbol_count;

	//populte the symbol table based on actions. for more detail view the programming language definition.	
	//start by looping the program array once again, we need a value to hold the next open location in symbol
	//	table, we will re-use symbol_count now that s_count permantly holds its value.
	symbol_count = 0;

	//cmd_count will serve to act as the 'count' variable for symbol table entries
	//opening and closing statements for loop and conditional statements will have the same count value
	//while symbol_count is updated after every entry in the symbol table, cmd_count will only be updated
	//after a command group is completely filled (i.e when the end if for an if structure is logged)
	int cmd_count =0;

	//This is the main loop responsible for filling the symbol table
	for(int i=0;i<p_count;i++){
		//-after each sucessfull entry in the symbol table symbol_count needs to be incremented
		//-for detailed information about preparser actions, consult the programming language definition
		switch(action_to_int(actions[ppa[i].action_index].action)){
			case DECLARE: 
				strcpy(st[symbol_count].name, get_value(ppa[i].variables,NAME).c_str());//load name
				st[symbol_count].value = atof(get_value(ppa[i].variables,VALUE).c_str());//load value
				st[symbol_count].location = i;		//load location
				st[symbol_count].count = cmd_count; //load count
				ppa[i].symbol_index = symbol_count;
				cmd_count++;
				symbol_count++;
				break;
			//evaluate commands (if, elseif, and for) will have symbol table entries based on their link properties
			case EVALUATE:
				int next_link = i;
				//resolve the links
				do{
					ppa[next_link].symbol_index = symbol_count;
					strcpy(st[symbol_count].name,actions[ppa[next_link].action_index].command);//load name
					st[symbol_count].location = next_link;	//load location
					next_link = get_next_link(actions[ppa[next_link].action_index].command,actions[ppa[next_link].action_index].link,next_link);
					st[symbol_count].value = next_link;	//load value
					st[symbol_count].count = cmd_count;	//load count
					symbol_count++;
				}while(strcmp(actions[ppa[next_link].action_index].link,"nill")!=0);
				//closing statement entries in the symbol table (eif, efor, etc)
				strcpy(st[symbol_count].name,actions[ppa[next_link].action_index].command);//load name
				st[symbol_count].location = next_link;	//load location
				st[symbol_count].value = i;				//back to the top for the value
				st[symbol_count].count = cmd_count;		//load count
				ppa[next_link].symbol_index = symbol_count;
				symbol_count++;
				cmd_count++;
				break;
			case GAMESPEC:
				strcpy(st[symbol_count].name,"gamespec");
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
		if(strcmp(actions[get_action(program[pcount])].command,command.c_str())==0){
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
						if(strcmp(actions[i].link,"nill")==0){			
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
}

/*parse
* parse the array of code lines
*/
void Parser::parse(){
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
				pc+=1;
				break;
			case DISPLAY:
				if(strcmp(actions[get_action(program[pc])].command,"disp")==0){
					s1 = get_value(ppa[pc].variables,NAME);//value name
					for(int i=0;i<s_count;i++){
						//if the name field from the program line and the symbol table name are the same					
						s2 = string(st[i].name);//symbol table name
						if(s1.compare(s2)==0){
							//update the value in the symbol table
							cout << "msso: " << st[i].value<<"\n";
							break;
						}
					}
				}
				else if(strcmp(actions[get_action(program[pc])].command,"write")==0){
					cout << "msso: " << get_value(ppa[pc].variables,NAME)<<"\n";
				}
				pc+=1;
				break;
			default:
				pc+=1;
		}
	}
}

/* lookup_eval
*	lookup the values of variables in the string vars, then evaluate the string returing an integer value
*/
float Parser::lookup_eval(string vars){
	string::size_type loc;
	char op;
	string s1;
	string s2;
	float var1;
	float var2;
	for(int i = 0;i<vars.length();i++){
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
/*_____---Main---_____
*---------------------
*---------------------
*/
int main(int argc, char ** argv){
	ifstream prog_fp;
	char in[256];
	int lc = 0;
	int temp=0;
	string file_path;
	if(argc == 1){
		cout << "msserr: Must specify a file name\n";
		return 0;
	}
	else{
		file_path = argv[1];
	}
	//open program
	prog_fp.open(file_path.c_str(),ifstream::in);
	//count lines that are not commented (don't begin with #)
	while(prog_fp.getline(in,256)){
		if(in[0] != '#'){
			lc++;
		}
	}
	//rewind config file
	prog_fp.clear();
	prog_fp.seekg(0,ios::beg);
	//allocate program space
	string program[lc];
	temp = lc; 
	//load the program
	lc=0;
	while(prog_fp.getline(in,256)){
		if(in[0] != '#'){
			//trimming
			string s = in;
			s=s.substr(0,s.find_last_not_of(' '));
			program[lc]=s;
			lc++;
		}
	}
	//cleanup
	prog_fp.close();
	Parser p(program,temp,"actions.cfg");
}
