#pragma once

#include <cmath>
#include <vector>
#include <boost/math/distributions/normal.hpp>
#include <limits>

class Asset
{
	const double _coeffM;
	const double _coeffX;
	const double _weight;
	std::vector<double> _defaultQuantiles;

public:

	Asset(double coeffM, double weight, const std::vector<double>& defaultProba) :
		_coeffM(coeffM), _weight(weight), _coeffX(sqrt(1 - pow(coeffM, 2)))
	{
		for (auto proba : defaultProba)
		{
			if (proba > 0){
				_defaultQuantiles.push_back(quantile(boost::math::normal(), proba));
			} else {
				_defaultQuantiles.push_back(std::numeric_limits<double>::lowest());
			}
		}
	}

	bool hasDefaulted(double X,double M, int date)
	{
		return _coeffX*X + _coeffM*M < _defaultQuantiles[date];
	}


	double getCoeffX() const
	{
		return _coeffX;
	}

	double getWeight() const
	{
		return _weight;
	}

	double getQuantile(int dateIndex)
	{
		return _defaultQuantiles[dateIndex];
	}

	~Asset();
};

