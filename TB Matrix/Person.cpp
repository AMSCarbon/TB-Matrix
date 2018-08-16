#include <Windows.h>
#include <iostream>
#include "Person.h"
#include "Globals.h"


Person::Person()
{
	infected = false;
	infectious = false; 
	timesInfected = 0; 
	strain = 0;
}


Person::~Person()
{
}

void Person::update(float transmission)
{
	if (!infected) {
		if (transmission > infectionRate && timesInfected < 1) {
			//arbitrarily chosen values atm. 
			if (rand() % (int)(1/infectionRate  - transmission*transmission + timesInfected*1000) == 0) {
				//the person is now infected with the sth strain. 
				infected = true;
			}
		}
	}

	//I-> R 
	if (infected) {
		if (rand() % (int)max(1, int(1/progressionRate)) == 0) {
			infectious = true; 
		}
	}

	//I-> R 
	if (infectious) {
		//probability function taking in transmission value and recovery rate value. higher values of both should make the event true more
		if (rand() % (int)max(1, (int)(1 / recoveryRate - timesInfected * 10)) == 0) {
			infected = false;
			infectious = false; 
			timesInfected++;
		}
	}
}

void Person::update(int strainNum, float transmission)
{
	if (!infected) {
		if (transmission > infectionRate && timesInfected < 1) {
			//arbitrarily chosen values atm. 
			if (rand() % (int)(1 / (infectionRate * pow(cost_of_resistance, strainNum)) - transmission*transmission + timesInfected * 1000) == 0) {
				//the person is now infected with the sth strain. 
				infected = true;
				strain = strainNum;
			}
		}
	}
	
	//I-> R 
	if (infected) {
		if (rand() % (int)max(1, int(1 / (progressionRate * pow(cost_of_resistance,strain))) ) == 0) {
			infectious = true;
		}
	}
	if (infected) {
		//mutate.
		if (rand() % (int)max(1, int(1 / mutationRate)) == 0) {
			if (strain == 0) strain++;
			else if (strain == numStrains) strain--;
			else {
				if (rand() % 2 == 0) strain++;
				else strain--;
			}

		}
	}
	//I-> R 
	if (infectious) {
		//probability function taking in transmission value and recovery rate value. higher values of both should make the event true more
		if (rand() % (int)max(1, (int)(1 / recoveryRate ) * pow(cost_of_resistance, strain) - timesInfected * 10) == 0) {
			infected = false;
			infectious = false;
			timesInfected++;
		}
	}
}

void Person::update(float* transmission)
{
	//S -> L
	if (!infected) {
		for (int s = 0; s < numStrains; s++) {
			if (transmission[strain] > infectionRate) {
				if (rand() % int((1 / infectionRate * 1 / pow(cost_of_resistance, s)) + 1000 * timesInfected) == 0) {
					//the person is now infected with the sth strain. 
					infected = true;
					strain = s;
				}
			}
		}
	}

	// L-> infected. 
	if (infected) {
		//probability function taking in transmission value and recovery rate value. higher values of both should make the event true more
		if (rand() % (int)(max(1, (int)(1 / recoveryRate - timesInfected * 10))) == 0) {
			infectious = true;
		}
	}
	//I-> R 
	if (infected) {
		//probability function taking in transmission value and recovery rate value. higher values of both should make the event true more
		if (rand() % (int)(max(1, (int)(1 / recoveryRate - timesInfected * 10)) ) == 0) {
			infected = false;
			infectious = false; 
		}
	}
}

void Person::setInfectious(bool S)
{
	infected = S;
}

bool Person::isInfectious()
{
	return infectious;
}

bool Person::isInfected() {
	return infected;
}
int Person::getStrainNum()
{
	return strain;
}
