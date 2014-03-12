#pragma once

#include <vector>
#include "Asset.h"

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

	const std::vector<Asset>& getAssets()
	{
		return _assets;
	}

	double computeSpread(const std::vector<double>& expectedLosses, double rate) const;
	double computeLoss(double loss) const;
};
