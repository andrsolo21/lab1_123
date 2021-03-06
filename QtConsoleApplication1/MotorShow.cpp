#include "MotorShow.h"
#include <fstream> 
#include <string>
#include <cstdio>
MotorShow::MotorShow()
{
	_head = NULL;
	_tail = NULL;
	_gabarits[0] = 10;
	_gabarits[1] = 10;
}

MotorShow::MotorShow(Car * addData[], int addCount, float addGabarits[]) {
	_head = NULL;
	_tail = NULL;
	_grr = 0;
	if (addGabarits[0] > 0 && addGabarits[1] > 0) {
		_gabarits[0] = addGabarits[0];
		_gabarits[1] = addGabarits[1];
	}

	for (int i = 0; i < addCount; i++) {
 		addElement(*addData[i]);
	}
}

MotorShow::MotorShow(const MotorShow & addData) {
	_head = NULL;
	_tail = NULL;
	_gabarits[0] = addData.getGabarits(0);
	_gabarits[1] = addData.getGabarits(1);

	for (int i = 0; i < addData.getCount(); i++) {
		addElement(addData[i]);
	}
}

MotorShow::MotorShow(std::string name) {
	//TODO
	_head = NULL;
	_tail = NULL;
	ifstream file(name);
	if (file.is_open())
	{
		file >> _gabarits[0];
		file >> _gabarits[1];
		std::string s;
		float size[2], coord[2], angle;
		Car * tempCar;
		for (file >> s; !file.eof(); file >> s) {
			file >> size[0] >> size[1] >> angle >> coord[0] >> coord[1];
			tempCar = new Car(s, angle, size, coord);
			addElement(*tempCar);
			delete tempCar;
		}
		file.close();
	}
	
}

int MotorShow::getCount() const {
	return (_tail - _head);
}

float MotorShow::getGabarits(int i) const {
	return _gabarits[i % 2];
}

/*Car MotorShow::getElement(int c) const {
	if (c < (_tail - _head) && c >= 0) {
		return *(_head + c);
	}
	else
		throw std::exception("index out of range");
}*/

Car MotorShow::operator [](int c) const
{
	if (c < (_tail - _head) && c >= 0) {
		return *(_head + c);
	}
	else
		throw std::exception("index out of range");
}

void MotorShow::addElement(Car  element) {
	if (element.isInit() && checkCar(element)) {
		grow10();
		Car * tempCar = new Car(element);
		*(_tail) = *(tempCar);
		delete tempCar;
		_tail = _tail + 1;
	}
}

void MotorShow::deleteElement(int c) {
	if (c < (_tail - _head) && c >= 0) {
		for (auto i = _head + c; i < _tail-1; i++) 
			*i = *(i + 1);
		_tail--;
	}
}

void MotorShow::deleteAll() {
	_tail = _head;
}

void MotorShow::printToFile(std::string f) const {
	ofstream fout(f, ios_base::out | ios_base::trunc);	
	fout << _gabarits[0] << ' ';
	fout << _gabarits[1] ;
	Car * tempCar;
	for (auto i = _head; i < _tail; i++) {
		fout <<endl<< i->getName() << endl;
		fout << i->getSize(0) << ' ' << i->getSize(1) << ' ' << i->getAngle()<<
			' '<< i->getCoord(0) << ' '<< i->getCoord(1);
	}
	fout.close();
}

void MotorShow::grow10() {
	if (_grr == 0 || _tail - _head == 10 * _grr - 1) {
		int startNum = 0;
		_grr++;
		Car * add = new Car[10 * _grr];
		if (_grr == 1) {
			_head = add;
			_tail = add;
		}
		else {
			int count = _tail - _head;
			for (auto i = 0; i < _tail - _head; i++) {
				*(add + i) = *(_head + i);
				delete (_head + i);
			}
			_head = add;
			_tail = add + count;
		}
	}
}

bool MotorShow::checkCar(const Car carToCheck) {
	float d;
	bool flag1, flag2,flag = true;
	float dots1[4][2],right,top;
	for (int j = 0; j < 2; j++) {
		dots1[0][j] = carToCheck.getA(j);
		dots1[1][j] = carToCheck.getB(j);
		dots1[2][j] = carToCheck.getC(j);
		dots1[3][j] = carToCheck.getD(j);
	}
	for (int j = 0; j < 4; j++) {
		if (dots1[j][0] >= _gabarits[0])
			return false;
		if (dots1[j][1] >= _gabarits[1])
			return false;
	}

	for (Car * i = _head; i < _tail; i++) {
	
		d = sqrt((i->getCoord(0) - carToCheck.getCoord(0)) * (i->getCoord(0) - carToCheck.getCoord(0)) +
			(i->getCoord(1) - carToCheck.getCoord(1)) * (i->getCoord(1) - carToCheck.getCoord(1)));
		if (i->getRSmall() + carToCheck.getRSmall() > d)
			flag = false;
		else {
			if (i->getRBig() + carToCheck.getRBig() < d) {
				//cout << " " << i->getName() << " " << i->getRBig() << " + " << carToCheck.getName() << " " << carToCheck.getRBig() << " = " << d << endl;
				flag = true;
			}
			else
			{
				float dots2[4][2];
				for (int j = 0; j < 2; j++) {
					dots2[0][j] = i->getA(j);
					dots2[1][j] = i->getB(j);
					dots2[2][j] = i->getC(j);
					dots2[3][j] = i->getD(j);
				}
				flag1 = dots(dots1, dots2);
				flag2 = dots(dots2, dots1);
				if (flag1 == false && flag2 == false)
					flag = false;
			}
		}
	}
	return flag;
}

float MotorShow::linear(float c[2], float a[2], float b[2]) {
	int mn = 1, x = 0, y = 0;
	if (b[0] != a[0])
		x = (c[0] - a[0]) / (b[0] - a[0]);
	else {
		x = 0;
		if (c[0] > b[0])
			mn = -1;
	}
	if (b[1] != a[1])
		y = (c[1] - a[1]) / (b[1] - a[1]);
	else {
		y = 0;
		if (c[1] > b[1])
			mn = -1;
	}
	return mn * (x - y);
}

int MotorShow::countDots(float a[2], float b[2], float rec[][2]) {
	int n = 0;
	float x;
	for (int i = 0; i < 4; i++) {
		x = linear(a, b, rec[i]);
		if (x <= 0)
			n = n - 1;
		else
			n = n + 1;
	}
	return n;
}

bool MotorShow::dots(float dots1[][2], float dots2[][2]) {
	int n1, n2, m1, m2;
	n1 = countDots(dots1[0], dots1[1], dots2);
	n2 = countDots(dots1[3], dots1[2], dots2);
	m1 = countDots(dots1[0], dots1[3], dots2);
	m2 = countDots(dots1[1], dots1[2], dots2);
	if ((n1 == -4 && n2 == -4) || (n1 == 4 && n2 == 4) || (m1 == -4 && m2 == -4) || (m1 == 4 && m2 == 4) || 1)
		return true;
	return false;
}

MotorShow::~MotorShow()
{
}