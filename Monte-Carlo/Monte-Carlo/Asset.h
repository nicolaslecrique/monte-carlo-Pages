#pragma once

#include <vector>
#include "Distribution.h"

/*
contains data specific to an asset
*/
class Asset
{
	//part of the asset correlated to the market
	const double _coeffM;
	//part of the asset independant of the market
	const double _coeffX;
	//weight of this asset in the CDO
	const double _weight;
	//portion of the asset recovered if asset default
	const double _recoveryRate;
	//default quantiles by coupon date
	std::vector<double> _defaultQuantiles;

public:

	Asset(double coeffM, double weight, double recoveryRate, const std::vector<double>& defaultProba, const Distribution& distrib);

	bool hasDefaulted(double X,double M, int date) const
	{
		return _coeffX*X + _coeffM*M < _defaultQuantiles[date];
	}


	double getCoeffX() const
	{
		return _coeffX;
	}

	double getCoeffM() const
	{
		return _coeffM;
	}

	double getWeight() const
	{
		return _weight;
	}

	double getQuantile(int dateIndex) const
	{
		return _defaultQuantiles[dateIndex];
	}

	double getRecoveryRate() const
	{
		return _recoveryRate;
	}

	~Asset();
};

