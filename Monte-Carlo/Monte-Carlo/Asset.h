#pragma once

#include <cmath>
#include <vector>
#include <assert.h>
#include <limits>
#include <memory>
#include "Distribution.h"

class Asset
{
	const double _coeffM;
	const double _coeffX;
	const double _weight;
	const double _recoveryRate;
	std::vector<double> _defaultQuantiles;

public:

	Asset(double coeffM, double weight, double recoveryRate, const std::vector<double>& defaultProba, const Distribution& distrib) :
		_coeffM(coeffM), _weight(weight), _recoveryRate(recoveryRate), _coeffX(sqrt(1 - pow(coeffM, 2)))
	{
		assert(std::is_sorted(defaultProba.begin(), defaultProba.end()));
		assert(_recoveryRate >= 0 && _recoveryRate <= 1);
		assert(_coeffM >= 0 && _coeffM <= 1);

		for (auto proba : defaultProba)
		{
			assert (proba >= 0 && proba <= 1);
			
			if (proba > 0 && proba < 1){
				double quantile = distrib.inverse_cumulative(proba);
				_defaultQuantiles.push_back(quantile);
				//
			} else if (proba == 0) {
				_defaultQuantiles.push_back(std::numeric_limits<double>::lowest());
			} else {
				_defaultQuantiles.push_back(std::numeric_limits<double>::max());
			}
		}
	}

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

