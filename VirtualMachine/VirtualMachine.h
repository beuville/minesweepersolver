#ifndef VIRTUALMACHINE_H_
#define VIRTUALMACHINE_H_

#include <string>
#include <windows.h>

#include "Parser.h"

using namespace std;

class VirtualMachine{
	public:
		VirtualMachine(string cfg_fp,string * pfiles,int pcount, int iterations);
		~VirtualMachine();
	private:
		double * performance;
};
#endif 
