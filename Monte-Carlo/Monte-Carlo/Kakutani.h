#pragma once

#include <vector>

class Kakutani
{
public:
	Kakutani(int base, bool halton, double left , double right);
	//Default Constructor use a new base at each construction
	Kakutani(double left = 0, double right = 1, bool halton = false);

	~Kakutani();

	double operator()();

private:
	const int _base;
	const double _left;
	const double _size;
	std::vector<int> _angle;
	std::vector<int> _current;
};

