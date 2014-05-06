#pragma once

#include <boost/math/distributions/normal.hpp>


class Distribution
{

public:

	virtual double inverse_cumulative(double proba) const = 0;

};

