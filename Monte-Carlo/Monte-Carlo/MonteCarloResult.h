#pragma once

#include <vector>

struct MonteCarloResult
{
	double Spread;
	std::vector<double> ExpectedLossByDate;
	std::vector<double> VarianceLossByDate;

	MonteCarloResult();
	~MonteCarloResult();
};

