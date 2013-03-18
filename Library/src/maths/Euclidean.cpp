#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include "Euclidean.h"

#define EXPORT __attribute__((visibility("default")))
using namespace std;

EXPORT
double Euclidean::Euclidean_distance(const vector<double> &actual, const vector<double> &training) {

	double distance = 0.0;

    for(unsigned i=0; (i < actual.size()); i++)
    {
        distance = pow((actual[i] - training[i]), 2);
    }

    return sqrt(distance);

}