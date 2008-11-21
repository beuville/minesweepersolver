#ifndef CREATESOLVER_H_
#define CREATESOLVER_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

class createSolver{
	public:
        void sCreate(string file, int seedUp);
	private:
        string getLine();
        string getWhile();
        string getOther();
};
#endif 
