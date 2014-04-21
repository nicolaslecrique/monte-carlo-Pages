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

using namespace std;



const Cdo BuildCdo(double k1, double k2, const Distribution& distribA)
{
	int nbAssets = 125;
	double lambda = 0.01;//default intensity
	double recoveryRate = 0.4;
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
	int nbSimu = 1000;
	init_alea();
	double rate = 0.01;

	MonteCarloCdoEngine engine;


	std::vector<double> k1Vect;
	std::vector<double> spreadVect;
	for (int i = 0; i < 10; i++)
	{
		double k1 = i/(double)10;
		double k2 = (i+1)/(double)10;

		const GaussianDistribution distribA;

		const Cdo cdo = BuildCdo(k1,k2, distribA);

		gaussian generatorM;
		gaussian generatorX;

		auto result = engine.Price(cdo, nbSimu, rate, generatorM, generatorX);
		double spread = result.Spread;
		k1Vect.push_back(k1);
		spreadVect.push_back(spread);
	}


 	std::ofstream outputFile;
  	outputFile.open ("test.data");
  	outputFile << "#k1 spread dummy" << endl;
  	for(int i = 0; i < k1Vect.size(); i++)
  	{
  		outputFile << k1Vect[i] << " " << spreadVect[i] << " " << cos(i) << endl;
  		std::cout << k1Vect[i] << " " << spreadVect[i] << endl;  
  	}
  	outputFile.close();

	return 0;
}

