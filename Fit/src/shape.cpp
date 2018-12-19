#pragma once
#include <cstdlib>
#include <stdlib.h>     /* srand, rand */
#include <string>
#include<string.h>
#include<iostream>
using namespace std;


class shape
{
public:

	int ID;
	int NumOfSides;
	float *Angles;
	std::string Name;
	bool isShown;

	//constructor
	shape(int ID , int numOfSides, std::string name)
	{
		this->ID = ID;
		this->Name = name;
		this->NumOfSides = numOfSides;
		this->isShown = true;
		gettingArrayOfAngles();
	};

	//gets all the possible accepted angles for the shape
	void gettingArrayOfAngles()
	{
		this->Angles = new float[this->NumOfSides];
		float angleStep = 360 / this->NumOfSides;
		float angle = 0;
		for (int i = 0; i < this->NumOfSides; i++)
		{
			this->Angles[i] = angle;
			angle += angleStep;
		}
	};

	//check if the angle of the shape =the angle of the hole
	bool onClick(int shapeAngle, int holeAngle)
	{
		for (int i = 0; i < this->NumOfSides; i++)
		{
			cout << "Angles: " << Angles[i] << endl;
			float diff = shapeAngle - (holeAngle + this->Angles[i]);
			if (abs(diff) <= 20)
			{
				cout << "i:" << i << endl << "shapeAngle" << shapeAngle << endl << "hole angle:" << holeAngle << endl;;
				return true;
			}	
		}
		return false;
	};
	

	~shape()
	{
	};
};

