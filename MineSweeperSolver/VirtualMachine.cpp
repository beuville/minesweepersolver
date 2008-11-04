#include <iostream>
#include <fstream>

#include "VirtualMachine.h"

/*constructor*/
VirtualMachine(string cfg_fp,string * pfiles,int pcount,int iterations){
	//initialize performance array
	performance = new double[pcount];
	for(int i =0;i<pcount;i++){
		//loop through all the iterations
		char in[256];
		int lc = 0;
		ifstream prog_fp;
		//open up the file
		prog_fp.open(pfiles[i].c_str(),ifstream::in);
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
		string * program = new string[lc];
		//load the program
		lc=0;
		while(prog_fp.getline(in,256)){
			if(in[0] != '#'){
				//trimming
				string s = in;
				s=s.substr(0,s.find_last_not_of(' ')+1);
				program[lc]=s;
				lc++;
			}
		}
		//cleanup
		prog_fp.close();
		//create thread
		double average = 0;
		double single_run;
		//run through according to #of iterations
		for(int i=0;i<iterations;i++){
			//create a parser and initialize it
			//sum the total of %completes
			Parser p(program,lc,cfg_fp);
			single_run = p.parse();
			average += single_run;
			if(single_run >=95){
				cout << "run: " << i << " compeleted " << single_run <<"%\n";
			}
		}
		//complete the average calculation
		average = average / iterations;
		cout << "average performance: " << average << "%\n";
	}
		
}
/*destructor*/
VirtualMachine::~VirtualMachine(){
	delete[] performance;
}
/*
int main(int argc, char**argv){
	if(argc < 2){
		cout << "Needs # of iterations\n";
		return 0;
	}
	else{
		string * pfiles = new string[1];
		pfiles[0] = string("easy1.mss");
		VirtualMachine vm("actions.cfg",pfiles,1,atoi(argv[1]));
	}
	
}
*/
