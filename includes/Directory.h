#ifndef _Directory_h
#define _Directory_h

#include <vector>
#include <iostream>

using namespace std;

/*//////////////////////////////////////////
			DIRECTORY.H

*//////////////////////////////////////////

class Directory {

	public:

		Directory();
		Directory(string dirName);

		bool open(string dirName);
		bool open();

		vector<string> files();
		vector<double> open_training_1d(string filename);
		vector<vector<double> > open_training_2d(string filename);
		vector<double> save_training_1d(string filename);
		vector<double> save_training_2d(string filename);

	protected:

		string directory_name;
};
#endif