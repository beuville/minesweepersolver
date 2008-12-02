/*
NOTE:
make the variables unique for each file by perhaps parsing in the file name.

malkovich malkovich malkovich malkovich malkovich
 malkovich malkovich
  malkovich malkovich malkovich malkovich malkovich
 malkovich malkovich
 malkovich malkovich malkovich
  malkovich malkovich malkovich malkovich
*/
#include "createSolver.h"

#define BUFMAX 256
string conditions[] = {"<",">","=","#","$","!"};
string manips[] = {"+","-","*","/"};

void whileFound(ofstream *out, ifstream *input, int *num_of_lines, string *filestring);

//keeping track of all the variables used for while loops so that
//they don't step on each other
int numWhileVars;
int maxWhileVars;

//pther variable uses (ifs?)
int numVars;

int boardWidth = 9;
int boardHeight = 9;
int maxInt = 10;

void createSolver::sCreate(string file, int seedUp)
{    
    //initialize random seed
    srand(time(NULL) + seedUp);

    numWhileVars = 0;
    maxWhileVars = -1;
    numVars = 0;

    ofstream out(file.c_str());
	if(! out.good()){
			cout <<"error creating pop member: "<<file<<"\n";
			exit(-6);
	}

	//make 100 requests for lines
    for (int i=0; i<100; i++)
	{
    	out << getLine();
	}
	
    out.close();
}

//get "a line"
string createSolver::getLine(){
    //output string
    std:stringstream out;
    
    //randomly select a line
    switch(rand()%3){
        //ifs and picksqs
        case 0:
        case 1:
            out << getOther();
            break;
        //while loop
        case 2:
            out << getWhile();
            break;
    }
    //cout << out.str();
    return out.str();
}

string createSolver::getWhile(){
    //output string
    std:stringstream out;
    //determining if it is a < and increment or > and decrement
    int con = rand()%2;
    //initial value of the loopiong variable
    int val = rand()%maxInt;
    //target value for the looping value
    int val2 = rand()%maxInt;
    //less than and increment
    if(con){
        //take care of loops with nowhere to go
        if(val == 0)
            val = 1;
        //make sure the target value is in the correct direction
        if(val <= val2)
            val2 = val-1;
    } else if(!con){
        //take care of loops with nowhere to go
        if(val == maxInt-1)
            val = maxInt-2;
        //make sure the target value is in the correct direction
        if(val >= val2)
            val2 = val+1;
    }
	//Mark the beginning of the while loop
	out << "#while\n";

    if(numWhileVars>maxWhileVars){

        //initialize looping variabl
        out << "int wvar" << numWhileVars << "=" << val << endl;
        //open the while loop
        out << "while wvar" << numWhileVars << conditions[con] << val2 << endl;
        //set the loop to update each pass through
        out << "update wvar" << numWhileVars << "=wvar" << numWhileVars << manips[con] << "1\n";
        //mark the variable as used so no one edits it

        numWhileVars++;
        maxWhileVars++;
    }
    else{
        //initialize looping variabl
        out << "update wvar" << numWhileVars << "=" << val << endl;
        //open the while loop
        out << "while wvar" << numWhileVars << conditions[con] << val2 << endl;
        //set the loop to update each pass through
        out << "update wvar" << numWhileVars << "=wvar" << numWhileVars << manips[con] << "1\n";
        //mark the variable as used so no one edits it
        numWhileVars++;
    }


    //call for a new "line" inside the loop
    out << getLine();
    //end the loop
    out << "ewhile\n";
	out << "#ewhile\n";
    numWhileVars--;
    
    return out.str();
    //cout << out.str();
    //return "temp";
}

string createSolver::getOther(){
    //output string
    std:stringstream out;

    //temp variable to hold random variables throughout
    int var;
    //keep track of decision of "<" vs ">"
    int con;

    //pick a square
    if(rand()%3){
		out <<"#picsq selection\n";
        out << "picsq ";

        //if no loops are chilling about, make it all random
        if(numWhileVars==0){
            out << rand()%boardWidth << "," << rand()%boardHeight << endl;
        } else {
            //pick a random number within the bounds of the while numbers
            //for the x coordinate
            var = rand()%(numWhileVars+1);
            //if outside, make it random
            if(var == numWhileVars || var >= boardWidth)
                out << rand()%boardWidth << ",";
            //otherwise make it be the while variable number
            else
                out << "wvar" << var << ",";

            
            //pick a random number within the bounds of the while numbers
            //for the y coordinate
            var = rand()%(numWhileVars+1);
            //if outside, make it random
            if(var == numWhileVars || var >= boardHeight)
                out << rand()%boardHeight << endl;
            //otherwise make it be the while variable number
            else
                out << "wvar" << var << endl;
        }
    }
    //if statement
    else
    {
        //choose to either be "<" or ">"
        con = rand()%2;
        //half the time poll for a square's contents for comparison
        if(rand()%2){
            //find out the value of a square
            out << "getsq ";

            //if there is a loop variable alive
            if(numWhileVars)
            {
                //get a random loop variable
                var = rand()%(numWhileVars);
                //if beyond the range of loop variables, make it random
                if(var == numWhileVars-1 || var >= boardWidth)
                    out << rand()%boardWidth << ",";
                //otherwise grab the loop variable
                else
                    out << "wvar" << var << ",";

                //get a random loop variable
                var = rand()%(numWhileVars);
                //if beyond the range of loop variables, make it random
    //            if(var == numWhileVars-1 || var >= boardHeight)
                out << rand()%boardHeight << endl;
                //otherwise grab the loop variable
    //            else
    //                out << "wvar" << var << endl;
                //grab the square variable and store
                out << "int var" << (numVars) << "=GSRETURN" << endl;
                //out << "int var" << (numVars) << "=4" << endl;
                
                //check conditions ("<" and ">")
                out << "if var" << (numVars) << conditions[con];
                //get a random loop variable
                var = rand()%(numWhileVars);
                //if beyond the range of loop variables, make it random
                if(var == numWhileVars-1)
                    out << rand()%maxInt << endl;
                //otherwise grab the loop variable
                else
                    out << "wvar" << var << endl;
            //if no loop variables do some random shit
            } else {
                out << rand()%boardWidth << ",";
                out << rand()%boardHeight << endl;
                //out << "int var" << (numVars) << "=4" << endl;
                out << "if var" << (numVars) << conditions[con];
                out << rand()%maxInt << endl;
            }

            //up the var count so the next line can't mess with it
            numVars++;
            out << getLine();
            //half the time there's an else
            if(rand()%2)
            {
                out << "else\n";
                out << getLine();
            }
            out << "eif\n";
            //numVars--;
        //half the time there's no polling for a square's contents
        } else {
            //if loop vars
            if(numWhileVars)
            {
                //randomly select var for first loopvar
                var = rand()%numWhileVars;
                out << "if wvar" << var << conditions[con];
    
                //pick a random num for comparison
                var = rand()%(numWhileVars+1);
                //if beyond upper limit, set random number
                if(var == numWhileVars)
                    out << rand()%maxInt << endl;
                //otherwise make it be the loop variable
                else
                    out << "wvar" << var << endl;

                out << getLine();
                //half the time there's an else
                if(rand()%2)
                {
                    out << "else\n";
                    out << getLine();
                }
                out << "eif\n";
            }
        }
    }
    return out.str();
}

void createSolver::combine_best(vector<string> selected_files, mssv vars, int cur_gen){
	int num_of_lines = 0;
	stringstream out;
	int next_gen = cur_gen+1;
	srand(time(0));

	int rand_file1 = rand()%selected_files.size();
	int rand_file2 = rand()%selected_files.size();
	//In case the random numbers are equal, redo
	while(rand_file1 == rand_file2 && selected_files.size() > 1){
		rand_file2 = rand()%selected_files.size();
	}

	for(int i = 0; i < vars.population; i++){
		char file1[BUFMAX], file2[BUFMAX], newfile[BUFMAX];
		char buf1[BUFMAX], buf2[BUFMAX];

		ifstream input1(selected_files.at(rand_file1).c_str());
		ifstream input2(selected_files.at(rand_file2).c_str());
		
		_itoa_s(next_gen,buf1,10, 10);
		_itoa_s(i,buf2,10, 10);
		sprintf(newfile, "%s\\%s%s%s\\%s%s%s", vars.test_dir.c_str(), vars.test_dir.c_str(), "_gen_", buf1, "population_", buf2, ".mss", " ");
		ofstream out(newfile);

		if(!input1.good() || !input2.good() || !out.good()){
			cout <<"error creating pop member: "<<file1<< "/" <<file2<< "\n";
			exit(-10);
		}

		char file1buf[BUFMAX], file2buf[BUFMAX];
		while(!input1.eof() && !input2.eof()){
			int random = rand()%2;
			input1.getline(file1buf, BUFMAX);
			input2.getline(file2buf, BUFMAX);
			
			switch(random){
				case 0:
					if(file1buf[0] == '#'){
						if(strcmp(file1buf, "#picsq selection") == 0){
							out << file1buf << "\n";
							input1.getline(file1buf, BUFMAX);
							out << file1buf << "\n";
							num_of_lines += 2;
							skipSection(&input2);
						}else if(strcmp(file1buf, "#while") == 0){
							whileFound(&out, &input1, &num_of_lines, file1buf);
							skipSection(&input2);
						}
					}
					break;
				case 1:
					if(file2buf[0] == '#'){
						if(strcmp(file2buf, "#picsq selection") == 0){
							out << file2buf << "\n";
							input2.getline(file2buf, BUFMAX);
							out << file2buf << "\n";
							skipSection(&input1);
						}else if(strcmp(file2buf, "#while") == 0){
							whileFound(&out, &input1, &num_of_lines, file2buf);
							skipSection(&input1);
						}
					}
					break;
			}//end switch
		}//end while
		input1.close();
		input2.close();
		out.close();
	}//end for
}

void createSolver::whileFound(ofstream *out, ifstream *input, int *num_of_lines, char *filestring){
	int while_count = 1;
	(*out) << filestring << "\n";

	while(strcmp(filestring, "#ewhile") != 0 && while_count >= 1 && !(input->eof())){
		(*input).getline(filestring, BUFMAX);
		(*num_of_lines)++;
		if(strcmp(filestring, "#while") == 0){
			while_count++;
		}else if(strcmp(filestring, "#ewhile") == 0){
			while_count--;
		}
		(*out) << filestring << "\n";
	}
}

void createSolver::skipSection(ifstream *input){
	int while_count = 0;
	char skip[256];
	(*input).getline(skip, BUFMAX);

	if(skip[0] == '#'){
		if(strcmp(skip, "#while") == 0){
			while_count++;
			while(strcmp(skip, "#ewhile") && while_count > 1){
				(*input).getline(skip, BUFMAX);
				if(strcmp(skip, "#while") == 0){
					while_count++;
				}else if(strcmp(skip, "#ewhile") == 0){
					while_count--;
				}
			}	
		}else if(strcmp(skip, "#picsq selection") == 0){
			(*input).getline(skip, BUFMAX);
		}
	}
}
