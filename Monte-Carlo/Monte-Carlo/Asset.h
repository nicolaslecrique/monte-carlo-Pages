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
		//for each (double proba in defaultProba)//TODO mauvaise syntax foreach
		//{
			//_defaultQuantiles.push_back(quantile(boost::math::normal(), proba));
		//}
		for (auto proba : defaultProba)
		{
			_defaultQuantiles.push_back(quantile(boost::math::normal(), proba));
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

	double getQuantile(int dateIndex)
	{
		return _defaultQuantiles[dateIndex];
	}

	~Asset();
};

