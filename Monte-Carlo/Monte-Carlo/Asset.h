#pragma once

#include <cmath>
#include <vector>
#include <boost/math/distributions/normal.hpp>

class Asset
{
	const double _coeffM;
	const double _coeffX;
	std::vector<double> _defaultQuantiles;

public:

	Asset(double coeffM, const std::vector<double>& defaultProba) :
		_coeffM(coeffM), _coeffX(sqrt(1 - pow(coeffM, 2)))
	{
		for each (double proba in defaultProba)
		{
			_defaultQuantiles.push_back(quantile(boost::math::normal(), proba));
		}
	}

	double getCoeffX() const
	{
		return _coeffX;
	}

	double getQuantile(int dateIndex)
	{
		return _defaultQuantiles[dateIndex];
	}

	~Asset();
};

