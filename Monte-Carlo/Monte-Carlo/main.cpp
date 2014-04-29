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



const Cdo BuildCdo(double k1, double k2, const Distribution& distribA,
 int nbAssets, double lambda, double recoveryRate, double correlatedCoeff)
{

	std::vector<double> spreadPaimentDates{ 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5 };

	std::vector<Asset> assets;
	for (int iAsset = 0; iAsset < nbAssets; iAsset++)
	{
		std::vector<double> defaultProbas;
		for (auto date : spreadPaimentDates)
		{
			defaultProbas.push_back(1 - exp(-lambda*date));
		}
		Asset asset(correlatedCoeff, ((double)1) / nbAssets, recoveryRate, defaultProbas, distribA);
		assets.push_back(asset);
	}

	return Cdo(k1, k2, spreadPaimentDates, assets);
}

void SaveFile(std::string fileName, std::vector<MonteCarloResult>& results, std::string distrib, std::string uniformGenerator )
{
	std::ofstream outputFile;
  	outputFile.open (fileName);

	outputFile << "---Parameters----;NbSimu;rate;k1;k2;distribution;UniformGenerator;nbAsset" << 
	";defaultIntensity;recoveryRate;correlatedCoeff;---Results---;" <<
	"spread;MinSpread95;MaxSpread95;---IntermediaryResults---;" <<
	"espNum;espDen;varNum;varDen;" 
	<< endl;


	for ( auto result : results)
	{
		outputFile
		<< "------;"
		<< result.NbSimulations << ";"
		<< result.Rate << ";"
		<< result.PricedCdo->getK1() << ";"
		<< result.PricedCdo->getK2() << ";"
		<< distrib << ";"
		<< uniformGenerator << ";"
		<< result.PricedCdo->getAssets().size() << ";"
		<< result.DefaultIntensity << ";"
		<< result.PricedCdo->getAssets()[0].getRecoveryRate() << ";"
		<< result.PricedCdo->getAssets()[0].getCoeffM() << ";"
		<< "------;"
		<< result.Spread << ";"
		<< result.MinSpread95 << ";"
		<< result.MaxSpread95 << ";"
		<< "------;"
		<< result.SpreadNumerator << ";"
		<< result.SpreadDenominator << ";"
		<< result.VarianceNumerator << ";"
		<< result.VarianceDenominator << ";"
		<< endl;
	}

  	outputFile.close();
}

void TestByNbSimu()
{
	double rate = 0.01;
	double k1 = 0.1;
	double k2 = 0.2;
	int nbAssets = 125;
	double lambda = 0.1;//default intensity
	double recoveryRate = 0.4;
	double correlatedCoeff = 0.5;

	MonteCarloCdoEngine engine;

	std::vector<int> nbSimuVector;

	int currentValue = 100;
	while (currentValue < 1000000)
	{
		nbSimuVector.push_back(currentValue);
		currentValue*=2;
	}

	std::vector<MonteCarloResult> resultVect;
	for (int nbSimu : nbSimuVector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<uniform> generatorM;
		gaussian<uniform> generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;

		resultVect.push_back(result);
	}

	SaveFile("NbSimu-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");

}


void TestByNbSimuWithoutRecovery()
{
	double rate = 0.01;
	double k1 = 0.1;
	double k2 = 0.2;
	int nbAssets = 125;
	double lambda = 0.1;//default intensity
	double recoveryRate = 0;
	double correlatedCoeff = 0.5;

	MonteCarloCdoEngine engine;

	std::vector<int> nbSimuVector;

	int currentValue = 100;
	while (currentValue < 100000)
	{
		nbSimuVector.push_back(currentValue);
		currentValue*=2;
	}

	std::vector<MonteCarloResult> resultVect;
	for (int nbSimu : nbSimuVector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<uniform> generatorM;
		gaussian<uniform> generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;

		resultVect.push_back(result);
	}

	SaveFile("NbSimuZeoRecovery(For comparison with theoritical results)-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");

}



void TestByRecoveryRate()
{
	double rate = 0.01;
	double k1 = 0.1;
	double k2 = 0.2;
	int nbAssets = 125;
	double lambda = 0.1;//default intensity
	double correlatedCoeff = 0.5;
	int nbSimu = 20000;

	MonteCarloCdoEngine engine;

	std::vector<double> recoveryRateVector;

	double current = 0;
	while (current <= 1)
	{
		recoveryRateVector.push_back(current);
		current+=0.1;
	}

	std::vector<MonteCarloResult> resultVect;
	for (double recoveryRate : recoveryRateVector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<uniform> generatorM;
		gaussian<uniform> generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;

		resultVect.push_back(result);
	}

	SaveFile("recoveryRate-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");
}

void TestByTranche()
{
	double rate = 0.01;
	//double k1 = 0.1;
	//double k2 = 0.2;
	int nbAssets = 125;
	double lambda = 0.1;//default intensity
	double correlatedCoeff = 0.5;
	int nbSimu = 20000;
	double recoveryRate = 0.4;

	MonteCarloCdoEngine engine;

	std::vector<double> trancheK1Vector;

	double current = 0;
	while (current <= 0.9)
	{
		trancheK1Vector.push_back(current);
		current+=0.1;
	}

	std::vector<MonteCarloResult> resultVect;
	for (double k1 : trancheK1Vector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k1+0.1, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<uniform> generatorM;
		gaussian<uniform> generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;

		resultVect.push_back(result);
	}

	SaveFile("Tranche-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");
}

void TestByNbAssets()
{
	double rate = 0.01;
	double k1 = 0.1;
	double k2 = 0.2;
	//int nbAssets = 125;
	double lambda = 0.1;//default intensity
	double correlatedCoeff = 0.5;
	int nbSimu = 20000;
	double recoveryRate = 0.4;

	MonteCarloCdoEngine engine;

	std::vector<int> nbAssetsVector;

	int current = 1;
	while (current <= 10000)
	{
		nbAssetsVector.push_back(current);
		current*=5;
	}

	std::vector<MonteCarloResult> resultVect;
	for (double nbAssets : nbAssetsVector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<uniform> generatorM;
		gaussian<uniform> generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;

		resultVect.push_back(result);
	}

	SaveFile("NbAssets-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");
}

void TestByDefaultIntensity()
{
	double rate = 0.01;
	double k1 = 0.1;
	double k2 = 0.2;
	int nbAssets = 125;
	//double lambda = 0.1;//default intensity
	double correlatedCoeff = 0.5;
	int nbSimu = 20000;
	double recoveryRate = 0.4;

	MonteCarloCdoEngine engine;

	std::vector<double> defaultIntensityVector;

	double current = 0.001;
	while (current <= 10)
	{
		defaultIntensityVector.push_back(current);
		current*=5;
	}

	std::vector<MonteCarloResult> resultVect;
	for (double lambda : defaultIntensityVector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<uniform> generatorM;
		gaussian<uniform> generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;

		resultVect.push_back(result);
	}

	SaveFile("defaultIntensity-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");
}


void TestBycorrelatedCoeff()
{
	double rate = 0.01;
	double k1 = 0.1;
	double k2 = 0.2;
	int nbAssets = 125;
	double lambda = 0.1;//default intensity
	//double correlatedCoeff = 0.5;
	int nbSimu = 20000;
	double recoveryRate = 0.4;

	MonteCarloCdoEngine engine;

	std::vector<double> correlCoeffVector;

	double current = 0;
	while (current <= 1)
	{
		correlCoeffVector.push_back(current);
		current+=0.1;
	}

	std::vector<MonteCarloResult> resultVect;
	for (double correlatedCoeff : correlCoeffVector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<uniform> generatorM;
		gaussian<uniform> generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;

		resultVect.push_back(result);
	}

	SaveFile("correlatedCoeff-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");
}

void TestByInterestRate()
{
	//double rate = 0.01;
	double k1 = 0.1;
	double k2 = 0.2;
	int nbAssets = 125;
	double lambda = 0.1;//default intensity
	double correlatedCoeff = 0.5;
	int nbSimu = 20000;
	double recoveryRate = 0.4;

	MonteCarloCdoEngine engine;

	std::vector<double> rateVector;

	double current = -0.01;
	while (current <= 0.1)
	{
		rateVector.push_back(current);
		current+=0.01;
	}

	std::vector<MonteCarloResult> resultVect;
	for (double rate : rateVector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<uniform> generatorM;
		gaussian<uniform> generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;

		resultVect.push_back(result);
	}

	SaveFile("rate-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");
}

int main()
{

	init_alea();
	PrimeNumbersGenerator::fnGeneratePrimeList();

	//TestByNbSimu();
	//TestByRecoveryRate();
	//TestByTranche();
	//TestByNbAssets();
	//TestByDefaultIntensity();
	//TestBycorrelatedCoeff();
	TestByNbSimuWithoutRecovery();


	return 0;

	double rate = 0.01;
	int nbSimu = 1000;

	MonteCarloCdoEngine engine;


	std::vector<double> k1Vect;
	std::vector<MonteCarloResult> resultVect;



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

