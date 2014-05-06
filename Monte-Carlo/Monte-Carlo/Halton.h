#pragma once

#include "Kakutani.h"

//Generate uniform numbers based on the low discrepence Halton sequence
class Halton : public Kakutani
{
public:
	Halton(int base, double left, double right) : Kakutani(base, true, left, right) {}

	//Default Constructor using a new base at each new construction
	Halton(double left = 0, double right = 1) : Kakutani(left, right, true) {}
};