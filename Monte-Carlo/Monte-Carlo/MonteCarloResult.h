#pragma once

#include <vector>
class Cdo;

struct MonteCarloResult
{
	double Spread;
	std::vector<double> ExpectedLossesByDate;
	std::vector<double> VarianceLossesByDate;
	std::vector<double> DiscoutFactors;
	double SpreadNumerator;
	double SpreadDenominator;
	double VarianceNumerator;
	double VarianceDenominator;

	double MinSpread95;
	double MaxSpread95;

	const Cdo* PricedCdo;
	int NbSimulations;
	double Rate;

	double DefaultIntensity;

	MonteCarloResult();
	~MonteCarloResult();
};

