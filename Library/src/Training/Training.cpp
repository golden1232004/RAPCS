#include <vector>
#include <iostream>
#include <dirent.h>
#include <string>  
#include <fstream>
#include <sstream>
#include "Training.h"

using namespace std;

#define EXPORT __attribute__((visibility("default")))

static bool is_open = false;
static string directory_name;

static string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
EXPORT
Training::Training() {

}
EXPORT
Training::Training(string dirName) {
	if(directory.open(dirName)) {
		is_open = true;
		directory_name = dirName;
	}else{
	  is_open = false;
	}

}
EXPORT
bool Training::open(string dirName) {

	Directory* d = new Directory(dirName);
	if(d)
	{
		is_open = true;
	}else{
		is_open = false;
	}
}
EXPORT
bool Training::open() {
	if(is_open)
	  return true;
	else
	  return false;
}
EXPORT
vector<string> Training::files() {

	vector<string> files = directory.files();

	return files; 
}

EXPORT
vector<double> Training::read_1D() {

}
EXPORT
vector<vector<double> > Training::read_2D(string filename, string* feature_vectors) {

	ifstream infile(filename.c_str());
	if(!infile.is_open())
	{
		cerr << "Cannot open trainging file (" << filename << ")" << endl;
		cerr << "Make sure the file exists and you have the correct permissions";
		exit(0);
	}

	int m = 0;
	int n = 0;
	vector<vector<double> > data;

	string file_title;
	string speaker; 

	infile >> m;
	infile >> n;
	infile >> file_title;
	infile >> speaker;

	data.resize(m);
	for(unsigned i=0; (i < m); i++)
	{
		data[i].resize(n);
		for(unsigned j=0; (j < n); j++)
		{
			infile >> data[i][j];
		}
	}

	feature_vectors[0] = file_title;
	feature_vectors[1] = speaker;
	return data; 
}
EXPORT
vector<vector<double> > Training::read_2D(string filename) {

	ifstream infile(filename.c_str());
	if(!infile.is_open())
	{
		cerr << "Cannot open trainging file (" << filename << ")" << endl;
		cerr << "Make sure the file exists and you have the correct permissions";
		exit(0);
	}

	int m = 0;
	int n = 0;
	vector<vector<double> > data;

	string file_title;
	string speaker; 

	infile >> m;
	infile >> n;

	data.resize(m);
	for(unsigned i=0; (i < m); i++)
	{
		data[i].resize(n);
		for(unsigned j=0; (j < n); j++)
		{
			infile >> data[i][j];
			cout << data[i][j] << endl;
		}
	}
	return data; 
}

EXPORT
vector<double> Training::write_1D(string filename) {

}
EXPORT
bool Training::write_2D(vector<vector<double> > &training, sampleData &data) {

	if(!is_open)
	{
		exit(0);
	}

	vector<string> files = this->files();
	
	int maxNum = 0;
	int minNum = 0;
	for(unsigned i=0; (i < files.size()); i++)
	{
		string str = files[i];

		size_t pos = str.find('-');
		str = str.substr(pos + 1);
		int size = std::atoi(str.c_str());

		if(size > minNum)
		{
			maxNum = size + 1;
			minNum = size;
		}
	}
	string nextEntity = convertInt(maxNum);
	string fileName = directory_name + "training-" + nextEntity + ".txt";
	ofstream outfile (fileName.c_str());

	outfile << data.N << endl;
	outfile << data.M << endl;
	outfile << data.sample_name << endl;
	outfile << data.speaker << endl;

	for(unsigned i=0; (i < data.N); i++)
	{
		for(unsigned j=0; (j < data.M); j++)
		{
			outfile << training[i][j] << " ";

		}
		outfile << endl;
	}
	

}