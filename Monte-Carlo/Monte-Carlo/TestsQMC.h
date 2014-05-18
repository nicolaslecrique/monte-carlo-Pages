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
#include <boost/numeric/ublas/matrix.hpp>

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


};

void TestKakutaniCorrelationDim()
{
	int nbDim = 100;
	boost::numeric::ublas::matrix<double> corr(nbDim, nbDim);

	PrimeNumbersGenerator::getNextPrimeNumber(true);
	int nbSimu = 10000;

	vector<shared_ptr<Kakutani>> generators;
	for (int i = 0; i < nbDim; i++)
	{
		generators.push_back(std::shared_ptr<Kakutani>(new Kakutani()));
	}

	for (int i = 0; i < nbDim; i++){
		for (int j = 0; j < nbDim; j++){
			corr(i, j) = 0;
		}
	}

	std::vector<double> generatedValue(nbDim);

	for (int simu = 0; simu < nbSimu; simu++)
	{
		for (int dim = 0; dim < nbDim; dim++)
		{
			generatedValue[dim] = (*generators[dim])();
		}
		
		for (int i = 0; i < nbDim; i++){
			for (int j = 0; j < nbDim; j++){
				corr(i, j) = corr(i, j) + (generatedValue[i] - 0.5)*(generatedValue[j] - 0.5);
			}
		}
	}

	for (int i = 0; i < nbDim; i++){
		for (int j = 0; j < nbDim; j++){
			corr(i, j) = 12*corr(i, j)/(nbSimu-1);
		}
	}

	std::ofstream outputFile;
	outputFile.open("correl-kakutani.csv");

	for (int i = 0; i < nbDim; i++){
		for (int j = 0; j < nbDim; j++){
			outputFile << corr(i, j) << ";";
		}
		outputFile << endl;
	}

	outputFile.close();

}