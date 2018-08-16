#include<iostream>
#include<iomanip>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include<time.h>
#include "Globals.h"
#include"region.h"

const int size = 2500;//rowSize ^2 
const int rowSize = 50;
const int maxDistance = 4;

//declared as globals. Variables are declared to the heap rather than the stack, prevents memory overflow issues. 
// matrices are row x col

//The distribution matrix is symetrical about the diagonal
//however, in general it will be used as dM[from][to]
static float distributionMatrix[size][size];
static int strainMatrix[numStrains][size];
float transmissionMatrix[numStrains][size];

static float strainVector[size];
static float transmissionVector[size];

//helper functions to keep the main function neater. 
void updateTransmissionMatrix(region **regions);
void updateTransmissionVector(region **regions);
void printDistributionMatrix();
void printStrainMatrix();
void printTransmissionMatrix();
void printDebug();

int main() {
	std::cout << std::fixed;
	std::cout << std::setprecision(2);
	srand(time(NULL));
	std::cout << "start\n";
	region* regionLattice[size];	 // sum(r2.infected * Dist(r1,r2) )

	//initialise the region connectivity network, it's stored as just a vector for ease of calculation;
	for (int i = 0; i < rowSize; i++ ) {
		for (int j = 0; j < rowSize; j++) {
			//std::cout << i << " " << j << "\n";
			regionLattice[i*rowSize+j] = new region(500, 1, i, j); // 500 people, 1 person per unit space, (i,j)	
		}
	}	
	
	//initialise the distribution matrix. 
	//naive initialisation atm, will fix later
	int m, n; 
	for (int i = 0; i < size; i++) {
		//zero the entire row. 
		for (int j = 0; j < size; j++) {
			distributionMatrix[i][j] = 0.0f;
		}

		m = i / rowSize; // rounded down because int.
		n = i - m*rowSize;
		//fix this 'cause it doesn't actually work :Y 
		for (int j = -maxDistance; j  <= maxDistance; j++) {
			for (int k = -maxDistance; k <= maxDistance; k++) {
				int temp_m = m + j;
				int temp_n = n + k;		
				int index = rowSize*temp_m + temp_n; 
				//fuck you c++
				if (j == 0 && k == 0) continue; //skip dist, it's just the current index
				if (index >= 0 && index < size
					&& temp_n >=0 && temp_n <rowSize
					&& temp_m >= 0 && temp_m <rowSize ) {
					distributionMatrix[i][index] =  0.5 / sqrt(j*j + k*k);
				}
				//std::cout << m << " " << n << " : " << temp_m << " " << temp_n << " " << index << "\n";	
			}
		}	

		//set the diagonal to 1. 
		distributionMatrix[i][i] = 1;
	}

	regionLattice[40]->infectNIndividuals(1);
	std::cout << "Initialisation completed\n";

	int screensize = 600;
	float scale = screensize / rowSize; 
	sf::RenderWindow window(sf::VideoMode(screensize,screensize), "TB model");
	int step = 0;

	while (window.isOpen()){
		sf::Event event;
		//check for close. 
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				printDebug();
			}
		}
		window.clear();

		updateTransmissionMatrix(regionLattice); //major bottleneck in this sim.
		
		for (int i = 0; i < numStrains; i++ ) {
			for (int j = 0; j < size; j++) {
				regionLattice[j]->update(i , transmissionMatrix[i][j]);
			}
		}

		int totalInfected = 0;
		//draw the lattice
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < rowSize; j++) {
				sf::RectangleShape rectangle(sf::Vector2f(scale , scale));
				rectangle.setPosition(j * scale, i*scale);
				float red =		255 * ((float)regionLattice[i*rowSize + j]->getStrainInfected(0) / 10) ;
				float blue =	255 * ((float)regionLattice[i*rowSize + j]->getStrainInfected(1) / 10);
				float green =	255 * ((float)regionLattice[i*rowSize + j]->getStrainInfected(2) / 10);
				rectangle.setFillColor(sf::Color(red, 0 ,blue));
				window.draw(rectangle);
				totalInfected += regionLattice[i*rowSize + j]->getTotalInfected();
			}
		}
		window.display();
		std::cout << step << " " <<  totalInfected << std::endl;
		step++;
	}
	return 0; 
}


//make the program work with just the vector again??? wtf happened
void updateTransmissionVector(region **regions) {

	for (int region = 0; region < size; region++) {
		strainVector[region] = regions[region]->getTotalInfected();
		//std::cout << regions[region]->getTotalInfected() << std::endl;
	}

	//clear the transmissionVector;
	for (int j = 0; j < size; j++) {
			transmissionVector[j] = 0;	
	}
	
	for (int j = 0; j < size; ++j){
		for (int k = 0; k<size; ++k)
		{		// transmission[to] += strain[from] * distribution[from][to] 
				transmissionVector[j] += strainVector[k] * distributionMatrix[k][j];
				//std::cout << strainVector[k] << " x " << distributionMatrix[j][k]  << std::endl;
		}	
	}
}

void updateTransmissionMatrix(region **regions) {
	int totalInfectious = 0;
	//update strain matrix.
	for (int strain = 0; strain < numStrains; strain++ ) {
		for (int region = 0; region < size; region++) {

			strainMatrix[strain][region] = regions[region]->getStrainInfected(strain);
			totalInfectious += regions[region]->getStrainInfected(strain);
		}
	}
	if (totalInfectious == 0) return; 
	//clear the transmissionmatrix 
	for (int i = 0; i < numStrains; i++) {
		for (int j = 0; j < size; j++) {
			transmissionMatrix[i][j] = 0;
		}
	}

	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for (int i = 0; i < numStrains; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			for (int k = 0; k<size; ++k)
			{
				//transmission[strain][to] += strain[strain][from] * distribution[from][to] 
				if(distributionMatrix[k][j] != 0 ) transmissionMatrix[i][j] += (float)strainMatrix[i][k] * distributionMatrix[k][j];
			}
		}
	}
}

void printDistributionMatrix() {
	std::cout << "Distribution Matrix" << std::endl;
	for (int i = 0; i < size;  i++) {
		std::cout << "["; 
		for (int j = 0; j < size;  j++) {
			std::cout << distributionMatrix[i][j] << ", "; 
		}
		std::cout << "]\n";
	}
}

void printStrainMatrix() {
		
	std::cout << "Strain Matrix" << std::endl;
	for (int i = 0; i < numStrains; i++) {
		std::cout << "[";
		for (int j = 0; j < size; j++) {
			std::cout << strainMatrix[i][j] << ", ";
		}
		std::cout << "]\n";
	}
}

void printTransmissionMatrix() {	

	std::cout << "transmission Matrix" << std::endl;
	for (int i = 0; i < numStrains; i++) {
		std::cout << "[";
		for (int j = 0; j < size; j++) {
			std::cout << transmissionMatrix[i][j] << ", ";
		}
		std::cout << "]\n";
	}
}

void printDebug() {
	printDistributionMatrix();
	printStrainMatrix();
	printTransmissionMatrix();
	Sleep(10000);
}