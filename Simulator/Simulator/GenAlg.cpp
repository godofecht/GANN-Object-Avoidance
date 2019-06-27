#include "GenAlg.h"

vector <double> child_one;
vector <double> child_two;
vector <double> getChildOne(){ return child_one; };
vector <double> getChildTwo(){ return child_two; };


Children Crossover(vector <double> c1, vector <double> c2)
{
	vector <double> c3;
	vector <double> c4;
	int crossoverPoint = rand()%(c1.size()-1);
	for (int i = 0; i < c1.size(); i++)
	{
		if (i < crossoverPoint)
		{
			c3.push_back(c1[i]);
			c4.push_back(c2[i]);
		}
		else
		{
			c3.push_back(c2[i]);
			c4.push_back(c1[i]);
		}
	}
	child_one = c3; child_two = c4;
	Children C(c3, c4);
	return C;
}