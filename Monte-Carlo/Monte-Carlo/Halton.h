#pragma once

#include "Kakutani.h"

class Halton : public Kakutani
{
public:
	Halton(int base, double left, double right) : Kakutani(base, true, left, right) {}
	Halton(double left = 0, double right = 1) : Kakutani(left, right, true) {}
};