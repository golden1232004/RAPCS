#include <vector>
#include <iostream>
#include <dirent.h>
#include "Directory.h"

using namespace std;

static DIR *dir_point = NULL;
static bool is_open = false;

#define EXPORT __attribute__((visibility("default")))


EXPORT
Directory::Directory() {

}
EXPORT
Directory::Directory(string dirName) {

	this->directory_name = dirName; 

	DIR *directory;

	directory = opendir(this->directory_name.c_str());

	if(directory) {
		is_open = true;
		dir_point = directory;
	}else{
		is_open = false;
	}
}
EXPORT
bool Directory::open(string dirName) {

	Directory* d = new Directory(dirName);
	if(is_open)
	{
		return true;
	}else{
		return false; 
	}
}
EXPORT
bool Directory::open() {
	if(is_open)
	  return true;
	else
	  return false;
}
EXPORT
vector<string> Directory::files() {
	struct dirent *entry = NULL;
	if(!is_open) {
		cerr << "Cannot establish a connection to the Directory." << endl;
		cerr << "Check the file exists and the correct permission are granted." << endl;
		exit(0);
	}
	int i = 0;

	vector<string> DirectoryContains;

	while(entry = readdir(dir_point))
	{
		int pos = strlen(entry->d_name) - 4;
		if(!strcmp(&entry->d_name[pos], ".txt"))
		{
			DirectoryContains.push_back(entry->d_name);
		}
		/*else{
			cerr << "No files to display" << endl;
			break;
			exit(0);
		}*/
	}
	return DirectoryContains;
}

