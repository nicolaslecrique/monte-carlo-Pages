#pragma once

#include <iostream>
#include <cmath>
#include <memory>
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


std::vector<std::shared_ptr<var_alea<double>>> GenerateGaussianGeneratorVector(int size)
{
	std::shared_ptr<var_alea<double>> gaussianPtr(new gaussian<uniform>());
	return std::vector<std::shared_ptr<var_alea<double>>>(size, gaussianPtr);
}


void SaveFile(std::string fileName, std::vector<MonteCarloResult>& results, std::string distrib, std::string uniformGenerator )
{
	std::ofstream outputFile;
  	outputFile.open (fileName);

	outputFile << "---Parameters----;NbSimu;rate;k1;k2;distribution;UniformGenerator;nbAsset" <<
		";defaultIntensity;recoveryRate;correlatedCoeff;alphaNIG;betaNIG;---Results---;" <<
	"spread;MinSpread95;MaxSpread95;---IntermediaryResults---;" <<
	"espNum;espDen;varNum;varDen;" 
	<< endl;


	for ( auto result : results)
	{
		outputFile
		<< "------;"
		<< result.NbSimulations << ";"
		<< result.Rate << ";"
		<< result.K1 << ";"
		<< result.K2 << ";"
		<< distrib << ";"
		<< uniformGenerator << ";"
		<< result.NbAssets << ";"
		<< result.DefaultIntensity << ";"
		<< result.RecoveryRate << ";"
		<< result.CorrelationFactor << ";"
		<< result.alphaNIG << ";"
		<< result.betaNIG << ";"
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
	while (currentValue < 000)
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
		auto generatorX = GenerateGaussianGeneratorVector(nbAssets);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

		resultVect.push_back(result);
	}

	SaveFile("NbSimu-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");
}

void TestNIGByAlpha()
{
	double rate = 0.01;
	double k1 = 0.1;
	double k2 = 0.2;
	int nbAssets = 125;
	double lambda = 0.1;//default intensity
	double recoveryRate = 0.4;
	double correlatedCoeff = 0.5;
	double nbSimu = 10000;
	double beta = 0;

	MonteCarloCdoEngine engine;

	std::vector<double> alphaVect;

	double currentValue = 0.5;
	while (currentValue < 10)
	{
		alphaVect.push_back(currentValue);
		currentValue += 0.5;
	}

	std::vector<MonteCarloResult> resultVect;
	for (double alpha : alphaVect)
	{
		double gamma = sqrt(alpha*alpha-beta*beta);
		double alphaM = alpha;
		double betaM = beta;
		double muM = -beta*gamma*gamma / (alpha*alpha);
		double deltaM = gamma*gamma*gamma / (alpha*alpha);

		double alphaA = alphaM / correlatedCoeff;
		double betaA = betaM / correlatedCoeff;
		double muA = muM / correlatedCoeff;
		double deltaA = deltaM / correlatedCoeff;

		double decorrelCoeff = sqrt(1 - correlatedCoeff*correlatedCoeff);
		double alphaX = alphaA*decorrelCoeff;
		double betaX = betaA*decorrelCoeff;
		double muX = muA*decorrelCoeff;
		double deltaX = deltaA*decorrelCoeff;

		const NIGDistribution distribA(alphaA,betaA,muA,deltaA);

		const Cdo cdo = BuildCdo(k1, k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		normal_inverse_gaussian<uniform> generatorM(alphaM, betaM, muM, deltaM);

		std::shared_ptr<var_alea<double>> genXPtr(new normal_inverse_gaussian<uniform>(alphaX, betaX, muX, deltaX));
		std::vector<std::shared_ptr<var_alea<double>>> genXVect(nbAssets, genXPtr);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, genXVect);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;
		result.alphaNIG = alpha;
		result.betaNIG = beta;
		resultVect.push_back(result);
	}

	SaveFile("NbSimu-NIG-ByAlpha-Pseudo.csv", resultVect, "NIG", "pseudo-alea");
}



void TestNIGByBeta()
{
	double rate = 0.01;
	double k1 = 0.1;
	double k2 = 0.2;
	int nbAssets = 125;
	double lambda = 0.1;//default intensity
	double recoveryRate = 0.4;
	double correlatedCoeff = 0.5;
	double nbSimu = 10000;
	double alpha = 1;

	MonteCarloCdoEngine engine;

	std::vector<double> betaVect;

	double currentValue = -0.9;
	while (currentValue < 0.91)
	{
		betaVect.push_back(currentValue);
		currentValue += 0.1;
	}

	std::vector<MonteCarloResult> resultVect;
	for (double beta : betaVect)
	{
		double gamma = sqrt(alpha*alpha - beta*beta);
		double alphaM = alpha;
		double betaM = beta;
		double muM = -beta*gamma*gamma / (alpha*alpha);
		double deltaM = gamma*gamma*gamma / (alpha*alpha);

		double alphaA = alphaM / correlatedCoeff;
		double betaA = betaM / correlatedCoeff;
		double muA = muM / correlatedCoeff;
		double deltaA = deltaM / correlatedCoeff;

		double decorrelCoeff = sqrt(1 - correlatedCoeff*correlatedCoeff);
		double alphaX = alphaA*decorrelCoeff;
		double betaX = betaA*decorrelCoeff;
		double muX = muA*decorrelCoeff;
		double deltaX = deltaA*decorrelCoeff;

		const NIGDistribution distribA(alphaA, betaA, muA, deltaA);

		const Cdo cdo = BuildCdo(k1, k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		normal_inverse_gaussian<uniform> generatorM(alphaM, betaM, muM, deltaM);

		std::shared_ptr<var_alea<double>> genXPtr(new normal_inverse_gaussian<uniform>(alphaX, betaX, muX, deltaX));
		std::vector<std::shared_ptr<var_alea<double>>> genXVect(nbAssets, genXPtr);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, genXVect);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;
		result.alphaNIG = alpha;
		result.betaNIG = beta;
		resultVect.push_back(result);
	}

	SaveFile("NbSimu-NIG-ByBeta-Pseudo.csv", resultVect, "NIG", "pseudo-alea");
}






void TestKakutaniByNbSimu()
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

		gaussian<Kakutani> generatorM;
		std::vector<std::shared_ptr<var_alea<double>>> generatorX;
		for ( int iAsset = 0 ; iAsset < nbAssets ; iAsset++ )
		{
			generatorX.push_back(std::shared_ptr<var_alea<double>>(new gaussian<Kakutani>()));
		}

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

		resultVect.push_back(result);
	}

	SaveFile("NbSimu-Gaussian-Kakutani.csv", resultVect, "gaussian", "Kakutani");
}

void TestKakutaniOneDimensionNbSimu()
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
	while (currentValue <= 100000)
	{
		nbSimuVector.push_back(currentValue);
		currentValue*=2;
	}

	std::vector<MonteCarloResult> resultVect;
	for (int nbSimu : nbSimuVector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<Kakutani> generatorM;
		std::shared_ptr<var_alea<double>> gaussianPtr(new gaussian<Kakutani>());
		std::vector<std::shared_ptr<var_alea<double>>> generatorX(nbAssets, gaussianPtr);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

		resultVect.push_back(result);
	}

	SaveFile("NbSimu-Gaussian-Kakutani-oneDimension.csv", resultVect, "gaussian", "Kakutani-oneDimension");
}

void TestHaltonByNbSimu()
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
	while (currentValue <= 100000)
	{
		nbSimuVector.push_back(currentValue);
		currentValue*=2;
	}

	std::vector<MonteCarloResult> resultVect;
	for (int nbSimu : nbSimuVector)
	{
		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA, nbAssets, lambda, recoveryRate, correlatedCoeff);

		gaussian<Halton> generatorM;
		std::vector<std::shared_ptr<var_alea<double>>> generatorX;
		for ( int iAsset = 0 ; iAsset < nbAssets ; iAsset++ )
		{
			generatorX.push_back(std::shared_ptr<var_alea<double>>(new gaussian<Halton>()));
		}

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

		resultVect.push_back(result);
	}

	SaveFile("NbSimu-Gaussian-Halton.csv", resultVect, "gaussian", "Halton");
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
		auto generatorX = GenerateGaussianGeneratorVector(nbAssets);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

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
		auto generatorX = GenerateGaussianGeneratorVector(nbAssets);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

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
		auto generatorX = GenerateGaussianGeneratorVector(nbAssets);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k1+0.1;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

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
		auto generatorX = GenerateGaussianGeneratorVector(nbAssets);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

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
		auto generatorX = GenerateGaussianGeneratorVector(nbAssets);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

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
		auto generatorX = GenerateGaussianGeneratorVector(nbAssets);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

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
		auto generatorX = GenerateGaussianGeneratorVector(nbAssets);

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		result.DefaultIntensity = lambda;
		result.K1 = k1;
		result.K2 = k2;
		result.NbAssets = nbAssets;
		result.RecoveryRate = recoveryRate;
		result.CorrelationFactor = correlatedCoeff;

		resultVect.push_back(result);
	}

	SaveFile("rate-Gaussian-Pseudo.csv", resultVect, "gaussian", "pseudo-alea");
}