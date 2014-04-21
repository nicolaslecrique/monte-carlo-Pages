#pragma once


#include "Distribution.h"


class GaussianDistribution : public Distribution
{

public:
	GaussianDistribution()
	{
	}

	double density(double x) const
	{
	}

	double cumulative(double x) const
	{
		//integration numérique
	}

	double inverse_cumulative(double proba) const
	{
		return quantile(boost::math::normal(), proba);
	}

};