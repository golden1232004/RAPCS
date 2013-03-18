#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include "DTW.h"

#define EXPORT __attribute__((visibility("default")))
EXPORT
double DTW::Distance(const std::vector< std::vector<double> > &actual, const std::vector< std::vector<double> > &training)
{
	int m = actual.size();
    int n = training.size();

    double cost[m][n];

    cost[0][0] = Euclidean_distance(actual[0], training[0]);

    for(int i = 1; i < m; i++)
            cost[i][0] = cost[i-1][0] + Euclidean::Euclidean_distance(actual[i], training[0]);
    for(int j = 1; j < n; j++)
            cost[0][j] = cost[0][j-1] + Euclidean::Euclidean_distance(actual[0], training[j]);

    for(int i = 1; i < m; i++)
            for(int j = 1; j < n; j++)
                cost[i][j] = std::min(cost[i-1][j], std::min(cost[i][j-1], cost[i-1][j-1])) 
                    + Euclidean::Euclidean_distance(actual[i],training[j]);
    
    return cost[m-1][n-1];

}