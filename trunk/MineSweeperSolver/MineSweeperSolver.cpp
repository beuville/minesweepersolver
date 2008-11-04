//#include <string>
#include <iostream>
#include <fstream>
#include <direct.h>
#include "VirtualMachine.h"

int main(int argc, char ** argv){
	if(argc < 6){
		cout <<"Usage: ./mss <language def> <test name> <generations> <iterations> <population size>\n";
		return -1;
	}
	//file checking
	ifstream cfg_stream(argv[1]);
	if(! cfg_stream.good()){
		cout <<"could not open language def: " << argv[1] <<"\n";
		return -2;
	}
	cout <<"language definition loaded\n";
	cfg_stream.close();

	//build test directory structure
	int generations = atoi(argv[3]);
	if(_mkdir(argv[2]) < 0){
		cout <<"could not create test directory: " << argv[2]<<"\n";
		return -3;
	}
	cout <<"test dir created\n";
	string test_dir(argv[2]);
	test_dir.append("\\");

	string cur_dir;
	string * dirs = new string[generations];//array to hold the directories
	char buf[32];//buffer for _itoa_s
	for(int i=0;i<generations;i++){
		cur_dir = test_dir;
		cur_dir.append(argv[2]);
		cur_dir.append("_gen_");
		_itoa_s(i,buf,10);
		cur_dir.append(buf);
		if(_mkdir(cur_dir.c_str())<0){
			cout <<"error filling test directory: "<<cur_dir<<"\n";
			_rmdir(test_dir.c_str());
			return -4;
		}
		dirs[i] = cur_dir;
	}
	cout <<"test dir filled\n";
	//build sample population
	int population = atoi(argv[5]);
	string target_file;
	string * population_files = new string[population];//array of program names for virtual machine
	for(int i = 0;i<population;i++){
		target_file = dirs[0];
		target_file.append("\\");
		target_file.append(argv[2]);
		target_file.append("_pop_");
		_itoa_s(i,buf,10);
		target_file.append(buf);
		target_file.append(".mss");
		ofstream out(target_file.c_str());
		if(! out.good()){
			cout <<"error creating pop member: "<<target_file<<"\n";
			return 0;
		}
		population_files[i] = target_file;
		//fill target file
		//------------random file creation--------------
		//----------------------------------------------
			out <<"#pick squares sequentially starting at 0,0\n";
			out <<"int xpos=0\n";
			out <<"int ypos=0\n";
			out <<"while xpos<9\n";
			out <<"while ypos<9\n";
			out <<"picsq xpos,ypos\n";
			out <<"update ypos=ypos+1\n";
			out <<"ewhile\n";
			out <<"update ypos=0\n";
			out <<"update xpos=xpos+1\n";
			out <<"ewhile\n";
		//-------------------------------------------------
		//-------------------------------------------------
		out.close();
	}
	cout << "generation: " << 0 << " poplulated\n";
	//test sample population
	int iterations = atoi(argv[4]); 
	double * performance = new double[population];
	VirtualMachine vm(argv[1],population_files,population,iterations,performance);
	for(int i =0;i<population;i++){
		cout <<"average for prog: "<< i << " : " << performance[i] << " after " << iterations << " iterations\n";
	}
	//iterate
}