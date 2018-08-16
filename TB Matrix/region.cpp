#include "region.h"
#include "Globals.h"
#include <iostream>
#include <Windows.h>

region::region( int pop, float d,  int x_n, int y_n )
{
	density = d;
	population = pop; 
	x = x_n;
	y = y_n;
	for (int i = 0; i < pop; i++) {
		populationList.push_back(new Person());
	}
}


region::~region()
{
}

void region::update(float transmission){
	for (Person* p : populationList) {
		p->update(transmission);
	}
}

void region::update(int strainNum, float transmission)
{
	for (Person* p : populationList) {
		p->update(strainNum, transmission);
	}
}

//transition between the states but ensure a constant integer population size? 
void region::update(float* transmission) {
	for (Person* p : populationList) {
		p->update(transmission);
	}
}

void region::infectNIndividuals(int N)
{
	for (int i = 0; i < N; i++) {
		populationList[i]->setInfectious(true);
	}
}


//get the number of infections with a specific strain. 
int region::getStrainInfected(int strain)
{
	int total = 0;
	for (Person* p : populationList) {
		if (p->isInfectious() && p->getStrainNum() == strain ) total++;
	}
	return total;
}


int region::getTotalInfected(){
	int total = 0; 
	for (Person* p : populationList) {
		if (p->isInfectious()) total++;
	}
	return total;
}