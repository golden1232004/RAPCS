#include <vector>
#include <iostream>
#include <dirent.h>
#include "../Directory/Directory.h"

using namespace std;
struct sampleData {

            int N;
            int M;
            string sample_name;
            string speaker;
};

class Training {

	public:

		Training();
		Training(string dirName);

		bool open(string dirName);
		bool open();

		vector<string> files();

		vector<double> read_1D();
		vector<vector<double> > read_2D(string filename, string* feature_vectors);
		vector<vector<double> > read_2D(string filename);
		vector<double> write_1D(string filename);
		bool write_2D(vector<vector<double> >&training, sampleData &data);

	protected:

		Directory directory;


};