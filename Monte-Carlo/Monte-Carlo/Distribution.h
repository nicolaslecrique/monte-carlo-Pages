#pragma once

#include <boost/math/distributions/normal.hpp>


class Distribution
{

public:


	//double density(double x) const = 0;

	//double cumulative(double x)

	virtual double inverse_cumulative(double proba) const = 0;

	//double generate() const = 0;


};

