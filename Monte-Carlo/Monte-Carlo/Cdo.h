#pragma once

#include <vector>


class Cdo
{
private:
	const double _k1;
	const double _k2;
	const std::vector<double> _spreadPaimentDates;


public:
	Cdo(double k1, double k2, const std::vector<double>& spreadPaimentDates);
	~Cdo();

	double computeSpread(const std::vector<double>& expectedLosses, double rate) const;
	double computeLoss(double loss) const;
};
