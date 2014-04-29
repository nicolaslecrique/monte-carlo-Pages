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

	auto result = cdo.computeSpreadAndAnalysis(ExpectedlossInCDOByDate, variancelossInCDOByDate, rate);


	//with a=2.5 => phi(a)=0.9876 => P(|N(0,1)|<a) > 0.975 
	//=> give confidence interval to 0.975² = 0.95 for CDO
	double numInterval975 = 2.5*sqrt(result.VarianceNumerator/nbSimulations);
	double denomInterval975 = 2.5*sqrt(result.VarianceDenominator/nbSimulations);

	result.MinSpread95 = 
	(result.SpreadNumerator-numInterval975)/(result.SpreadDenominator+denomInterval975);

	result.MaxSpread95 = 
	(result.SpreadNumerator+numInterval975)/(result.SpreadDenominator-denomInterval975);

	result.PricedCdo = &cdo;
	result.NbSimulations = nbSimulations;
	result.Rate = rate;

	return result;
}