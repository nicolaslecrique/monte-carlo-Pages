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

	std::vector<double> lossInCDOByDate(cdo.getSpreadPaimentDates().size(),0);
	std::vector<double> variancelossInCDOByDate(cdo.getSpreadPaimentDates().size(),0);
	for (int iIter = 0; iIter < nbSimulations; iIter++)
	{
		int iDate = 0;
		double defaultedPortion = 0;
		for (auto date : cdo.getSpreadPaimentDates())
		{
			int iAsset = 0;
			double m = generatorM();
			for (const Asset& asset : cdo.getAssets())
			{
					double x = generatorX();
					if (asset.hasDefaulted(x, m, iDate))
					{
						defaultedPortion += asset.getWeight()*(1-asset.getRecoveryRate());
					}
				iAsset++;
			}
			double loss = cdo.computeLossInCdo(defaultedPortion);
			lossInCDOByDate[iDate] += loss;
			variancelossInCDOByDate[iDate] += loss*loss;
			iDate++;
		}

	}
	std::transform(lossInCDOByDate.begin(), lossInCDOByDate.end(), lossInCDOByDate.begin(),
		std::bind1st(std::multiplies<double>(), ((double)1) / nbSimulations));

	for (int i = 0 ; i < variancelossInCDOByDate.size() ; i++)
	{
		variancelossInCDOByDate[i] = variancelossInCDOByDate[i]/(nbSimulations-1) - 
		nbSimulations*lossInCDOByDate[i]*lossInCDOByDate[i]/(nbSimulations-1);
	}

	double spread = cdo.computeSpread(lossInCDOByDate, rate);

	MonteCarloResult result;
	result.Spread = spread;
	result.ExpectedLossByDate = lossInCDOByDate;
	result.VarianceLossByDate = variancelossInCDOByDate;

	return result;
}