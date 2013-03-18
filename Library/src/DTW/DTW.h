#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include "../maths/Euclidean.h"

using namespace std;
using namespace Euclidean;

namespace DTW {

	double Distance(const std::vector< std::vector<double> > &actual, const std::vector< std::vector<double> > &training);

}