#pragma once
class Person
{
private: 
	int strain;
	bool infected; // Latent
	bool infectious;  //disease 
	int timesInfected;
public:
	Person();
	~Person();
	void update(float transmission);
	void update(int strainNum, float transmission);
	void update(float* transmission ); //better oo
	void setInfectious(bool state);
	bool isInfected();
	bool isInfectious(); 
	int getStrainNum();
};

