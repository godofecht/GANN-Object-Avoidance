#ifndef GENALG_H
#define GENALG_H

#include "NN.h"
#include<vector>
using namespace std;

class Children
{
public:
	vector <double> childOne;
	vector <double> childTwo;

	Children(vector<double> c1, vector <double> c2)
	{
		childOne = c1; childTwo = c2;
	}
		

};


Children Crossover(vector <double> c1, vector <double> c2);

#endif