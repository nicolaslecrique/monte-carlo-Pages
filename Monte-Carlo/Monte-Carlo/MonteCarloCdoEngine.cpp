#include "MonteCarloCdoEngine.h"
#include "var_alea.h"

MonteCarloCdoEngine::MonteCarloCdoEngine()
{
}


MonteCarloCdoEngine::~MonteCarloCdoEngine()
{
}

const MonteCarloResult MonteCarloCdoEngine::Price(const Cdo& cdo, int nbSimulations, double rate) const
{
	gaussian gaussianGen;

	std::vector<double> lossInCDOByDate(cdo.getSpreadPaimentDates());
	for (int iIter = 0; iIter < nbSimulations; iIter++)
	{

		int iDate = 0;
		std::vector<bool> hasDefaulted(cdo.getAssets().size(), false);

		double defaultedPortion = 0;
		for (auto date : cdo.getSpreadPaimentDates())
		{
			int iAsset = 0;
			for (const Asset& asset : cdo.getAssets())
			{
				if (!hasDefaulted[iAsset])
				{
					double x = gaussianGen();
					double m = gaussianGen();
					if (asset.hasDefaulted(x, m, iDate))
					{
						hasDefaulted[iAsset] = true;
						defaultedPortion += asset.getWeight();
					}
				}
				iAsset;
			}
			lossInCDOByDate[iDate] += cdo.computeLossInCdo(defaultedPortion);
			iDate++;
		}

	}
	std::transform(lossInCDOByDate.begin(), lossInCDOByDate.end(), lossInCDOByDate.begin(),
		std::bind1st(std::multiplies<double>(), ((double)1) / nbSimulations));

	double spread = cdo.computeSpread(lossInCDOByDate, rate);

	MonteCarloResult result;

	result.Spread = spread;

	return result;
}