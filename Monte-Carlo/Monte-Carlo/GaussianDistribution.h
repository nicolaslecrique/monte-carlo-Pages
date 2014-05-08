#pragma once


#include "Distribution.h"
#include <boost/math/distributions/normal.hpp>

class GaussianDistribution : public Distribution
{

public:

	double inverse_cumulative(double proba) const
	{
		return quantile(boost::math::normal(), proba);
	}

};