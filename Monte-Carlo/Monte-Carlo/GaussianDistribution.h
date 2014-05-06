#pragma once


#include "Distribution.h"


class GaussianDistribution : public Distribution
{

public:

	double inverse_cumulative(double proba) const
	{
		return quantile(boost::math::normal(), proba);
	}

};