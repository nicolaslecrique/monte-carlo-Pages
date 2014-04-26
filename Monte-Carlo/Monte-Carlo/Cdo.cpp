#include "Cdo.h"
#include <assert.h>
#include <algorithm>
#include <functional>

Cdo::Cdo(double k1, double k2, const std::vector<double>& spreadPaimentDates, const std::vector<Asset>& assets)
: _k1(k1), _k2(k2), _spreadPaimentDates(spreadPaimentDates), _assets(assets)
{
	assert(k1 >=0);
	assert(k2 <= 1);
	assert(k2 > k1);
	assert(std::is_sorted(spreadPaimentDates.begin(), spreadPaimentDates.end()));
	double totalweight=0;
	for(auto asset : assets)
	{
		totalweight += asset.getWeight();
	}
	assert(totalweight >= 0.999);
	assert(totalweight <= 1.001);

}


Cdo::~Cdo()
{
}


MonteCarloResult Cdo::computeSpreadAndAnalysis(
	const std::vector<double>& expectedLosses,  const std::vector<double>& varianceLosses, double rate) const
{
	assert(expectedLosses.size() == _spreadPaimentDates.size());
	assert(varianceLosses.size() == _spreadPaimentDates.size());

	auto first = expectedLosses.begin();
	auto end = expectedLosses.end();

	double numerator = 0;
	double denominator = 0;

	double varianceNumerator = 0;
	double varianceDenominator = 0;

	std::vector<double> discountFactors;

	int dateIndex = 0;
	for (auto it = first; it < end; ++it, ++dateIndex)
	{
		double previousLost = (it == first) ? 0 : *(it - 1);
		double previousDate = (it == first) ? 0 : _spreadPaimentDates[dateIndex - 1];
		double discountFactor = exp(-rate*_spreadPaimentDates[dateIndex]);
		double nextDiscountFactor = (it == (end-1)) ? 0 : exp(-rate*_spreadPaimentDates[dateIndex+1]);
		double deltaT = _spreadPaimentDates[dateIndex] - previousDate;

		numerator += (*it - previousLost)*discountFactor;
		denominator += deltaT*(1. - *it)*discountFactor;

		varianceNumerator += (discountFactor-nextDiscountFactor)*(discountFactor-nextDiscountFactor)*varianceLosses[dateIndex];
		varianceDenominator +=  discountFactor*discountFactor*deltaT*deltaT*varianceLosses[dateIndex];

		discountFactors.push_back(discountFactor);
	}

	MonteCarloResult result;

	result.Spread = numerator/denominator;
	result.ExpectedLossesByDate = expectedLosses;
	result.VarianceLossesByDate = varianceLosses;
	result.SpreadNumerator = numerator;
	result.SpreadDenominator = denominator;
	result.VarianceNumerator = varianceNumerator;
	result.VarianceDenominator = varianceDenominator;
	result.DiscoutFactors = discountFactors;

	return result;
}


double Cdo::computeLossInCdo(double lossInAllAssets) const
{
	return std::max(std::min(lossInAllAssets, _k2) - _k1, 0.) / (_k2 - _k1);
}
