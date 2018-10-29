#include <QtCore/QCoreApplication>
#include "Car.h"
#include "MotorShow.h"
#include "assert.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream> 
using namespace std;
void checkCollection1(MotorShow * col1,MotorShow * col2) {
	assert(col1->getGabarits(0) == col2->getGabarits(0));
	assert(col1->getGabarits(1) == col2->getGabarits(1));
	assert(col1->getCount() == col2->getCount());
	for (int i = 0; i < col2->getCount(); i++) {
		assert((*col1)[i].getName() == (*col2)[i].getName());
		assert((*col1)[i].getAngle() == (*col2)[i].getAngle());
		assert((*col1)[i].getSize(0) == (*col2)[i].getSize(0));
		assert((*col1)[i].getSize(1) == (*col2)[i].getSize(1));
		assert((*col1)[i].getCoord(0) == (*col2)[i].getCoord(0));
		assert((*col1)[i].getCoord(1) == (*col2)[i].getCoord(1));
	}
}

bool checkCollection(MotorShow * col1, MotorShow * col2) {
	if (col1->getGabarits(0) != col2->getGabarits(0))
		return false;
	if (col1->getGabarits(1) != col2->getGabarits(1))
		return false;
	if (col1->getCount() != col2->getCount())
		return false;
	for (int i = 0; i < col2->getCount(); i++) {
		if((*col1)[i].getName() != (*col2)[i].getName()) return false;
		if((*col1)[i].getAngle() != (*col2)[i].getAngle()) return false;
		if((*col1)[i].getSize(0) != (*col2)[i].getSize(0)) return false;
		if((*col1)[i].getSize(1) != (*col2)[i].getSize(1)) return false;
		if((*col1)[i].getCoord(0) != (*col2)[i].getCoord(0)) return false;
		if((*col1)[i].getCoord(1) != (*col2)[i].getCoord(1)) return false;
	}
	return true;
}

void printCollection(MotorShow * col) {
	for (int i = 0; i < col->getCount(); i++) {
		cout << "Num " << i << endl;
		cout << (*col)[i].getName() << endl <<
			(*col)[i].getSize(0) << ' '<< (*col)[i].getSize(1)
			<< ' ' << (*col)[i].getAngle() << ' ' << (*col)[i].getCoord(0) << ' ' <<
			(*col)[i].getCoord(1) << endl << endl;
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	MotorShow * myCollection = new MotorShow;
	assert(myCollection->getCount() == 0);
	assert(myCollection->getGabarits(0) == 10);
	assert(myCollection->getGabarits(1) == 10);
	delete myCollection;

	float gabarits[2];
	ifstream file("file.txt");
	if (!file.is_open()) 
		return -1;
	string s;
	float size[2], coord[2], angle;
	Car * tempCar[10], CheckElement;
	int n = 0;
	file >> gabarits[0];
	file >> gabarits[1];
	for (file >> s; !file.eof(); file >> s) {
	//while(getline(file,s)){
		//getline(file, s);
		file >> size[0] >> size[1] >> angle >> coord[0] >> coord[1];
		tempCar[n] = new Car(s,angle,size,coord);
		n++;
	}
	file.close();

	//конструктор инициализации
	myCollection = new MotorShow(tempCar,n,gabarits);
	printCollection(myCollection);
	assert(myCollection->getGabarits(0) == gabarits[0]);
	assert(myCollection->getGabarits(1) == gabarits[1]);
	assert(myCollection->getCount() == n);
	for (int i = 0;i < n;i++) {
		CheckElement = (*myCollection)[i];
		assert(CheckElement.getName() == tempCar[i]->getName());
		assert((*myCollection)[i].getAngle() == tempCar[i]->getAngle());
		assert((*myCollection)[i].getSize(0) == tempCar[i]->getSize(0));
		assert((*myCollection)[i].getSize(1) == tempCar[i]->getSize(1));
		assert((*myCollection)[i].getCoord(0) == tempCar[i]->getCoord(0));
		assert((*myCollection)[i].getCoord(1) == tempCar[i]->getCoord(1));
	}

	myCollection->printToFile("printBefore.txt");

	//конструктор копирования
	MotorShow * myNewCollection = new MotorShow(*myCollection);	
	assert(checkCollection(myNewCollection, myCollection));

	//добавление элемента
	float size1[2], coord1[2];
	size1[0] = 2;
	size1[1] = 5;
	coord1[0] = 1;
	coord1[1] = 8;
	Car addCar("nameAdd", 30, size1, coord1);

	n = myNewCollection->getCount();
	myNewCollection->addElement(addCar);
	assert(myNewCollection->getCount() == n+1);
	assert(!checkCollection(myNewCollection, myCollection));
	//printCollection(myNewCollection);

	//удаление элемента
	n = myNewCollection->getCount();
	myNewCollection->deleteElement(2);
	assert(myNewCollection->getCount() == n - 1);
	assert(!checkCollection(myNewCollection, myCollection));
	//printCollection(myNewCollection);
	
	//выгрузка в файл
	myNewCollection->printToFile("printAfter.txt");

	//удаление всех
	myCollection->deleteAll();
	assert(myCollection->getCount() == 0);


	delete myCollection;
	myCollection = new MotorShow("printAfter.txt");
	printCollection(myCollection);
	myCollection->printToFile("printAfter2.txt");
	assert(checkCollection(myCollection, myNewCollection));
	delete myNewCollection;
	delete myCollection;
	return 0;
}
