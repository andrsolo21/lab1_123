#pragma once
#include "Car.h"
#include <fstream> 
#include <string>
#include <iostream>
using namespace std;
class MotorShow
{
	
public:
	struct Rect {
		Rect * next, *last;
		Car *data;
	};
	MotorShow();
	MotorShow(Car * addData[], int addCount, float addGabarits[]);
	MotorShow(const MotorShow & addData);
	MotorShow(std::string f);
	~MotorShow();

	void addElement(const Car element);
	void deleteElement(int i);	
	void deleteAll();
	void printToFile(std::string f) const;

	//Car getElement(int i) const;
	Car operator[](int c) const;

	int getCount() const;
	float getGabarits(int i) const;


private:
	float _gabarits[2];
	Car *_head, *_tail;
	int _grr = 0;

	void grow10();
	bool checkCar(const Car carToCheck);
	float linear(float x[2], float a[2], float b[2]);
	int countDots(float a[2], float b[2],float rec[][2]);
	bool dots(float dots1[][2], float dots2[][2]);
	
};

