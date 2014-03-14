#pragma once

#include "MonteCarloResult.h"
#include "Cdo.h"

class MonteCarloCdoEngine
{
public:
	MonteCarloCdoEngine();

	const MonteCarloResult Price(const Cdo&, int nbSimulations, double rate) const;

	~MonteCarloCdoEngine();
};

