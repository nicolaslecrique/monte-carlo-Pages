#include "Kakutani.h"
#include "PrimeNumbersGenerator.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Kakutani::Kakutani(int base, bool halton, double left, double right) :
 _base(base), _left(left), _size(right-left)
{
	double y = 1/(double)base;
	double x = halton ? y : 1/5.;

	//to maintain precision whatever the base is and optimise computation time 
	int nbDigits = std::max(5,(int)(log(65536)/log(base)));

	for (int i = 0 ; i < nbDigits ; i++)
	{
		x*=base;
		int xFloor = (int)x;
		_current.push_back(xFloor);
		x-=xFloor;

		y*=base;
		int yFloor = (int)y;
		_angle.push_back(yFloor);
		y-=yFloor;
	}
}


Kakutani::Kakutani(double left, double right, bool halton) : Kakutani(PrimeNumbersGenerator::getNextPrimeNumber(),halton,left,right) {}


Kakutani::~Kakutani()
{
}

double Kakutani::operator()()
{
	int remaining = 0;
	double result = 0;
	double basePower = (double)_base;

	for (int i = 0; i < _angle.size(); i++)
	{
		int temp = _current[i] + _angle[i] + remaining;
		_current[i] = temp%_base;
		remaining = temp / _base;
		result += _current[i]/basePower;
		basePower*=_base;
	}
	return _left + result*_size;
}

