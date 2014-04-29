#pragma once

#include <vector>
#include "Asset.h"
#include "MonteCarloResult.h"

class Cdo
{
private:
	const double _k1;
	const double _k2;
	const std::vector<double> _spreadPaimentDates;
	const std::vector<Asset> _assets;


public:
	Cdo(double k1, double k2, const std::vector<double>& spreadPaimentDates, const std::vector<Asset>& assets);
	~Cdo();

	const std::vector<Asset>& getAssets() const
	{
		return _assets;
	}

	const std::vector<double>& getSpreadPaimentDates() const
	{
		return _spreadPaimentDates;
	}

	const double getK1() const
	{
		return _k1;
	}

	const double getK2() const
	{
		return _k2;
	}


	MonteCarloResult computeSpreadAndAnalysis(const std::vector<double>& expectedLosses, const std::vector<double>& varianceLosses, double rate) const;
	double computeLossInCdo(double lossInAllAssets) const;
};
