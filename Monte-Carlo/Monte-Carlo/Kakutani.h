#pragma once

#include <vector>

class Kakutani
{
public:
	Kakutani(int base);

	~Kakutani();

	double Next();

private:
	const int _base;
	std::vector<int> _angle;
	std::vector<int> _current;
};

