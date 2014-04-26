#include "MonteCarloCdoEngine.h"


MonteCarloCdoEngine::MonteCarloCdoEngine()
{
}


MonteCarloCdoEngine::~MonteCarloCdoEngine()
{
}

const MonteCarloResult MonteCarloCdoEngine::Price(
	const Cdo& cdo, int nbSimulations, double rate, var_alea<double>& generatorM, var_alea<double>& generatorX) const
{
	gaussian gaussianGen;

	std::vector<double> ExpectedlossInCDOByDate(cdo.getSpreadPaimentDates().size(),0);
	std::vector<double> variancelossInCDOByDate(cdo.getSpreadPaimentDates().size(),0);
	for (int iIter = 0; iIter < nbSimulations; iIter++)
	{
		int iDate = 0;
		for (auto date : cdo.getSpreadPaimentDates())
		{
			double m = generatorM();
			double defaultedPortion = 0;
			for (const Asset& asset : cdo.getAssets())
			{
				double x = generatorX();
				if (asset.hasDefaulted(x, m, iDate))
				{
					defaultedPortion += asset.getWeight()*(1-asset.getRecoveryRate());
				}
			}
			double loss = cdo.computeLossInCdo(defaultedPortion);
			ExpectedlossInCDOByDate[iDate] += loss;
			variancelossInCDOByDate[iDate] += loss*loss;
			iDate++;
		}

	}
	std::transform(ExpectedlossInCDOByDate.begin(), ExpectedlossInCDOByDate.end(), ExpectedlossInCDOByDate.begin(),
		std::bind1st(std::multiplies<double>(), ((double)1) / nbSimulations));

	for (int i = 0 ; i < variancelossInCDOByDate.size() ; i++)
	{
		variancelossInCDOByDate[i] = variancelossInCDOByDate[i]/(nbSimulations-1) - 
		nbSimulations*ExpectedlossInCDOByDate[i]*ExpectedlossInCDOByDate[i]/(nbSimulations-1);
	}

	return cdo.computeSpreadAndAnalysis(ExpectedlossInCDOByDate, variancelossInCDOByDate, rate);
}