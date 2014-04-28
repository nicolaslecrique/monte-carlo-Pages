#include <iostream>
#include <cmath>
#include "Cdo.h"
#include "Asset.h"
#include "MonteCarloCdoEngine.h"
#include "var_alea.h"
#include "NIGDistribution.h"
#include "GaussianDistribution.h"
#include <iostream>
#include <fstream> 
#include "Kakutani.h"
#include "Halton.h"
#include "PrimeNumbersGenerator.h"

using namespace std;



const Cdo BuildCdo(double k1, double k2, const Distribution& distribA)
{
	int nbAssets = 125;
	double lambda = 0.2;//default intensity
	double recoveryRate = 0;
	std::vector<double> spreadPaimentDates{ 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5 };

	std::vector<Asset> assets;
	for (int iAsset = 0; iAsset < nbAssets; iAsset++)
	{
		std::vector<double> defaultProbas;
		for (auto date : spreadPaimentDates)
		{
			defaultProbas.push_back(1 - exp(-lambda*date));
		}
		Asset asset(0.5, ((double)1) / nbAssets, recoveryRate, defaultProbas, distribA);
		assets.push_back(asset);
	}

	return Cdo(k1, k2, spreadPaimentDates, assets);
}


int main()
{

	init_alea();
	PrimeNumbersGenerator::fnGeneratePrimeList();


	double rate = 0.02;
	int nbSimu = 1000;

	MonteCarloCdoEngine engine;


	std::vector<double> k1Vect;
	std::vector<MonteCarloResult> resultVect;
	for (int i = 2; i < 3; i++)
	{
		double k1 = i/(double)10;
		double k2 = (i+1)/(double)10;

		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA);

		gaussian<Kakutani> generatorM;
		gaussian<Kakutani> generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		k1Vect.push_back(k1);
		resultVect.push_back(result);
	}


 	std::ofstream outputFile;
  	outputFile.open ("test.data");
  	outputFile << "#k1 spread dummy" << endl;
  	for(int i = 0; i < k1Vect.size(); i++)
  	{

  		auto result = resultVect[i];

  		outputFile << k1Vect[i] << " " << resultVect[i].Spread << " " << cos(i) << endl;
  		std::cout 
  		<< "K1:" << k1Vect[i]  << ";	"
  		<< "Spread:" << result.Spread*100 <<"\%;	"
  		<< "espNum:" << result.SpreadNumerator << ";	"
  		<< "espDen:" << result.SpreadDenominator << ";	"
  		<< "varNum:" << result.VarianceNumerator << ";	"
  		<< "varDen:" << result.VarianceDenominator << ";	"
  		<< "MinSpread95%" << result.MinSpread95 << ";	"
  		<< "MaxSpread95%" << result.MaxSpread95 << ";	"
  		<< std::endl << "ExpectedLossesByDate:";
  		for (auto loss : result.ExpectedLossesByDate)
  		{
  			std::cout << loss << ";";
  		}
  		std::cout << endl << "VarianceLossesByDate:";
  		for (auto var : result.VarianceLossesByDate)
  		{
  			std::cout << var << ";";
  		}
  		std::cout << endl << "DiscoutFactors:";
  		for (auto discount : result.DiscoutFactors)
  		{
  			std::cout << discount << ";";
  		}
  		std::cout << endl;
  	}
  	outputFile.close();

return 0 ;

	Kakutani k;
	double sum = 0;
	for ( int i = 1 ; i < 10000 ; i++){
		double random = k();
		sum+= random;

		if ( (i%1000) != 0) continue;
		std::cout << "random:" << random << std::endl;
		std::cout << "mean:" << sum/i << std::endl;

	}

	return 0;
}

