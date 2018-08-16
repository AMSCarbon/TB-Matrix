#pragma once
#include <Vector>
#include "Person.h"
class region
//change this to individual based rather than floats. 
{

private: 
	std::vector<Person*> populationList;
	float density; 
	int population; 
	int x; //position on the grid. 
	int y; 
public:
	region(int pop, float d, int x_n, int y_n  );
	~region();
	void update(float transmission);

	void update(int strainNum, float transmission);
	void update(float* transmission);
	void infectNIndividuals(int N);
	int getTotalInfected();
	int getStrainInfected(int strain);

};

