#pragma once

#include <vector>

//Generate uniform numbers based on the low discrepence Kakutani sequence
class Kakutani
{
public:
	Kakutani(int base, bool halton, double left , double right);

	//Default Constructor using a new base at each new construction
	Kakutani(double left = 0, double right = 1, bool halton = false);

	~Kakutani();

	//generate next number in the sequence
	double operator()();

private:
	const int _base;
	const double _left;
	const double _size;
	//contains anlge and current number in base _base
	//{2,5,4,3,0,4} means 0,254304
	std::vector<int> _angle;
	std::vector<int> _current;
};

