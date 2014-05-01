#pragma once

#include "MonteCarloResult.h"
#include "Cdo.h"
#include "var_alea.h"

class MonteCarloCdoEngine
{
public:
	MonteCarloCdoEngine();

	const MonteCarloResult Price(const Cdo&, int nbSimulations, double rate, var_alea<double>& generatorM,  std::vector<std::shared_ptr<var_alea<double>>>& generatorXByAssetS) const;

	~MonteCarloCdoEngine();
};

