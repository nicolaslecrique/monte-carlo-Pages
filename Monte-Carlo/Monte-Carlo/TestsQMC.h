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



void TestQmcSequences()
{
	std::vector<double> meanByPrimeKakutani;
	std::vector<double> secondMomentByPrimeKakutani;
	PrimeNumbersGenerator::getNextPrimeNumber(true);
	int nbSimu = 1000000;
	for ( int indexPrimeNumber = 0 ; indexPrimeNumber < 500 ; indexPrimeNumber++ )
	{
		Kakutani k;
		double mean = 0 ;
		double secondMoment = 0;
		for(int i = 0 ; i < nbSimu ; i++)
		{
			double value = k();
			mean += value;
			secondMoment += value*value;
		}
		mean/=nbSimu;
		secondMoment/=nbSimu;
		meanByPrimeKakutani.push_back(mean);
		secondMomentByPrimeKakutani.push_back(secondMoment);
	}



	std::vector<double> meanByPrimeHalton;
	std::vector<double> secondMomentByPrimeHalton;
	PrimeNumbersGenerator::getNextPrimeNumber(true);
	for ( int indexPrimeNumber = 0 ; indexPrimeNumber < 500 ; indexPrimeNumber++ )
	{
		Halton k;
		double mean = 0 ;
		double secondMoment = 0;
		for(int i = 0 ; i < nbSimu ; i++)
		{
			double value = k();
			mean += value;
			secondMoment += value*value;
		}
		mean/=nbSimu;
		secondMoment/=nbSimu;
		meanByPrimeHalton.push_back(mean);
		secondMomentByPrimeHalton.push_back(secondMoment);
	}

	uniform u;
	double mean = 0 ;
	double secondMoment = 0;
	for(int i = 0 ; i < nbSimu ; i++)
	{
		double value = u();
		mean += value;
		secondMoment += value*value;
	}
	mean/=nbSimu;
	secondMoment/=nbSimu;

	std::ofstream outputFile;
  	outputFile.open ("qualite-QMC.csv");

	outputFile << 
	"primeIndex;kakutaniEsp;kakutaniSecondMoment;haltonEsp;haltonSecondMoment;uniformEsp;uniformSecMoment" << endl;


	for ( int i = 0 ; i < 500 ; i++)
	{
		outputFile
		<< i << ";"
		<< meanByPrimeKakutani[i] << ";"
		<< secondMomentByPrimeKakutani[i] << ";"
		<< meanByPrimeHalton[i] << ";"
		<< secondMomentByPrimeHalton[i] << ";"
		<< mean << ";"
		<< secondMoment << ";"
		<< endl;
	}

  	outputFile.close();


}
