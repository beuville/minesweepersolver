#ifndef CREATESOLVER_H_
#define CREATESOLVER_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

using namespace std;

//From MineSweeperSolver.cpp.
struct mssv{
	string test_dir;
	string lang_def;
	int population;
	int iterations;
	int generations;
	double threshold;
	double mutation;
	double combination;
};

class createSolver{
	public:
        void sCreate(string file, int seedUp);
		void combine_best(vector<string> selected_files, mssv vars, int cur_gen);
	private:
        string getLine();
        string getWhile();
        string getOther();
		void whileFound(ofstream *out, ifstream *input, int *num_of_lines, string *filestring);
};
#endif 
