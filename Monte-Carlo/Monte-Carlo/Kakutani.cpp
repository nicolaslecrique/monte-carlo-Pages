#include "Kakutani.h"
#include "PrimeNumbersGenerator.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Kakutani::Kakutani(int base, bool halton, double left, double right) :
 _base(base), _left(left), _size(right-left)
{
	double baseDouble = (double)base;
	double y = 1./baseDouble;
	double x = halton ? y : 1./5.;

	//to maintain precision whatever the base is and optimise computation time
	//nbDigits n is solution of base^n = 2^16 : 16 bits recision
	int nbDigits = std::max(5,(int)(log(65536)/log(base)));

	//algorithm compute base-adique decomposition.
	//first step in base 5 : 0.2435;
	//0.2435*5 => 2.435, then we take 2 by taking integer part and so on
	for (int i = 0 ; i < nbDigits ; i++)
	{
		x*=baseDouble;
		//Tricks to prevent numerical instability: (1/base)*base = 0.9999999 for certain bases
		//which leads to a bug when taking integer part
		int xFloor = (int)(x+0.000001);
		_current.push_back(xFloor);
		x-=xFloor;

		y*=baseDouble;
		int yFloor = (int)(y+0.000001);
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

	//in line alogihm to do two things in the same time:
	//-1-Compute next number
	//-2-Compute double number from base-adic decomposition
	//Addition algorithm but from left to right taking remaining to the right
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

