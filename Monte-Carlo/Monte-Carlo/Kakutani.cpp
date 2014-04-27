#include "Kakutani.h"
#include <iostream>

Kakutani::Kakutani(int base) : _base(base)
{
	double y = 1/(double)base;
	double x = 1/5.;

	for (int i = 0 ; i < 32 ; i++)
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

Kakutani::~Kakutani()
{
}

double Kakutani::Next()
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
	return result;
}

