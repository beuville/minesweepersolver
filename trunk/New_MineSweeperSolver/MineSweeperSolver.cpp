//#include <string>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <dirent.h>
#include <direct.h>

#include "VirtualMachine.h"
#include "createSolver.h"

using namespace std;

double stddev(double * nums,double mean,int count);
double mean(double * nums,int count);
bool DeleteDirectory(string path);

createSolver solver;


void fill_file(string file, int seedUp);
vector<string> next_generation(int gen, string * gen_path, string * prev_gen, double * performance,mssv vars);
	
int main(int argc, char ** argv){
	if(argc < 2){
		cout <<"Usage: ./mss <config file>\n";
		return -1;
	}
	//file checking
	ifstream cfg_stream(argv[1]);
	if(! cfg_stream.good()){
		cout <<"could not open config file: " << argv[1] <<"\n";
		return -2;
	}

	//---------load configuration---------
	string lang_def, test_dir, game_diff, s_generations, s_iterations, s_population, s_threshold, s_mutation, s_combination;
	int generations, iterations, population;
	double threshold, mutation, combination;
	//lang_def
	cfg_stream >> lang_def;
	cfg_stream >> lang_def;
	//test_dir
	cfg_stream >> test_dir;
	cfg_stream >> test_dir;
	//game_diff
	cfg_stream >> game_diff;
	cfg_stream >> game_diff;
	//generations
	cfg_stream >> s_generations;
	cfg_stream >> s_generations;
	generations = atoi(s_generations.c_str());
	//interations
	cfg_stream >> s_iterations;
	cfg_stream >> s_iterations;
	iterations = atoi(s_iterations.c_str());
	//population
	cfg_stream >> s_population;
	cfg_stream >> s_population;
	population = atoi(s_population.c_str());
	//threshold
	cfg_stream >> s_threshold;
	cfg_stream >> s_threshold;
	threshold=atof(s_threshold.c_str());
	//mutation
	cfg_stream >> s_mutation;
	cfg_stream >> s_mutation;
	mutation = atof(s_mutation.c_str());
	//combination
	cfg_stream >> s_combination;
	cfg_stream >> s_combination;
	combination = atof(s_combination.c_str());

	mssv vars;
	vars.test_dir = test_dir;
	vars.lang_def = lang_def;
	vars.population = population;
	vars.iterations = iterations;
	vars.generations = generations;
	vars.threshold = threshold;
	vars.mutation = mutation;	
	vars.combination = combination;
	cout <<"config file loaded\n";
	cfg_stream.close();

	/*If test directory exists, delete
	struct stat dir_exist;
	if(stat(test_dir.c_str(), &dir_exist) == 0){
		//if(!DeleteDirectory(test_dir.c_str())){
			//cout <<"could not delete test directory\n";
			//return -8;
		//}
		Directory::Delete(test_dir, true);
		cout <<"directory deleted\n";
	}*/

	//build test directory structure
	if(_mkdir(test_dir.c_str()) < 0){
		cout <<"could not create test directory: " << test_dir <<"\n";
		return -3;
	}
	cout <<"test dir: "<< test_dir << " created\n";

	string cur_dir;
	string * dirs = new string[generations];//array to hold the directories
	char buf[32];//buffer for _itoa_s
	//create generation folders
	for(int i=0;i<generations;i++){
		cur_dir = test_dir;
		cur_dir.append("\\");
		cur_dir.append(test_dir);
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
	string target_file;
	string * population_files = new string[population];//array of program names for virtual machine
	for(int i = 0;i<population;i++){
		target_file = dirs[0];
		target_file.append("\\population_");
		_itoa_s(i,buf,10);
		target_file.append(buf);
		target_file.append(".mss");
		population_files[i] = target_file;
		cout << target_file << endl;
		//fill target file
		fill_file(target_file, i);
	}
	cout << "generation: " << 0 << " poplulated\n";
	double * performance = new double[population];
	//test sample populations
	for(int i=0;i<generations;i++){
		VirtualMachine vm(lang_def,population_files,population,iterations,performance);

		cout <<"Generation " << i << " completed\n";
		vector<string> selected_files = next_generation(i,dirs,population_files,performance,vars);
		solver.combine_best(selected_files, vars, i);
	}
	cout <<"Press any key to exit...\n";
	getchar();
	return 0;
}

//
bool DeleteDirectory(string path){
	DIR *dirPtr;
	struct dirent *entryPtr;
	struct stat statBuf;
	char cCurrentPath[_MAX_PATH];

	dirPtr = opendir(path.c_str());
	if(!dirPtr){
		perror("Could not open path:");
		return false;
	}

	if (!getcwd(cCurrentPath, _MAX_PATH))
		return false;

	int i = 0;
	//Checks the size of argv
	while(cCurrentPath[i] != '\0'){
		i++;
	}
	//If the last element in argv is not a /, add one
	if(cCurrentPath[i-1] != '\\'){
		cCurrentPath[i] = '\\';
	}


	while(entryPtr = readdir(dirPtr)){
		char buffer[_MAX_PATH];
		stat(entryPtr->d_name, &statBuf);
		
		if(strcmp(entryPtr->d_name, "..") && strcmp(entryPtr->d_name, ".")){
			sprintf(buffer, "%s%s", cCurrentPath, entryPtr->d_name);
			if((statBuf.st_mode) & S_IFDIR && strcmp(entryPtr->d_name, ".")){
				if(!DeleteDirectory(buffer)){
					return false;
				}
			}else{
				remove(buffer);
			}
		}
	}
	closedir(dirPtr);
	return rmdir(path.c_str());
}


void fill_file(string file, int seedUp){
    solver.sCreate(file, seedUp);

	//ofstream out(file.c_str());
	//if(! out.good()){
	//		cout <<"error creating pop member: "<<file<<"\n";
	//		exit(-6);
	//}

	////------------random file creation--------------
	////----------------------------------------------
	//out <<"#pick squares sequentially starting at 0,0\n";
	//out <<"int xpos=0\n";
	//out <<"int ypos=0\n";
	//out <<"while xpos<9\n";
	//out <<"while ypos<9\n";
	//out <<"picsq xpos,ypos\n";
	//out <<"update ypos=ypos+1\n";
	//out <<"ewhile\n";
	//out <<"update ypos=0\n";
	//out <<"update xpos=xpos+1\n";
	//out <<"ewhile\n";
	////-------------------------------------------------
	////-------------------------------------------------
	//out.close();
}

double mean(double * nums, int count){
	double sum=0;
	for(int i = 0;i<count;i++){
		sum+=nums[i];
	}
	return sum / count;
}

double stddev(double * nums,double mean,int count){
	//standard deviation
	double sum=0;
	double mean_dev;
	//mean deviations
	sum = 0;
	for(int i = 0;i<count;i++){
		mean_dev = nums[i] - mean;
		//mean squared deviations
		mean_dev = mean_dev * mean_dev;
		sum += mean_dev;
	}
	sum = sum / count;
	double std_dev = sqrt(sum);
	return std_dev;
}

//Returns a vector of all selected files going to next generation
vector<string> next_generation(int gen, string * gen_path,string * prev_gen, double * performance, mssv vars){
	//------------statistics calculation------------------
	//----------------------------------------------------
	vector<string> selected_files;
	string stats_path = gen_path[gen];
	stats_path.append("\\stats.txt");
	ofstream stats_file(stats_path.c_str());
	if(! stats_file.good()){
		cout <<"error opening stats file\n";
		exit(-7);
	}
	double mn = mean(performance,vars.population);
	double std_dev = stddev(performance,mn,vars.population );
	stats_file << "Mean: " << mn << "\n";
	stats_file << "Standard deviation: " << std_dev << "\n\n";
	stats_file << "Population performance ( ** denotes selection)\n";
	//loop through programs and select any that preformed above mean + (threshold * std_dev) for the next generation
	for(int i = 0 ;i<vars.population;i++){
		stats_file << prev_gen[i] << ": " << performance[i] << " %";
		if(performance[i] >= mn + (vars.threshold * std_dev)){
			//push selected files on vector
			selected_files.push_back(prev_gen[i]);
			stats_file << " **";
		}
		stats_file << "\n";
	}
	return selected_files;
}
