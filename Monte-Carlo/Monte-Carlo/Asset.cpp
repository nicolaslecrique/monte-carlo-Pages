

#include "Asset.h"
#include <assert.h>
#include <algorithm>
#include <limits>
#include <cmath>
#include <memory>


Asset::~Asset()
{
}

Asset::Asset(double coeffM, double weight, double recoveryRate, const std::vector<double>& defaultProba, const Distribution& distrib) :
	_coeffM(coeffM), _weight(weight), _recoveryRate(recoveryRate), _coeffX(sqrt(1 - pow(coeffM, 2)))
{
	assert(std::is_sorted(defaultProba.begin(), defaultProba.end()));
	assert(_recoveryRate >= 0 && _recoveryRate <= 1);
	assert(_coeffM >= 0 && _coeffM <= 1);

	for (auto proba : defaultProba)
	{
		assert (proba >= 0 && proba <= 1);
		
		//specific treatment of 0 and 1 to prevent error in inversion
		if (proba > 0 && proba < 1){
			double quantile = distrib.inverse_cumulative(proba);
			_defaultQuantiles.push_back(quantile);
		} else if (proba == 0) {
			_defaultQuantiles.push_back(std::numeric_limits<double>::lowest());
		} else {
			_defaultQuantiles.push_back(std::numeric_limits<double>::max());
		}
	}
}