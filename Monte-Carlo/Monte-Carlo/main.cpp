#include <iostream>
#include <cmath>
#include "Cdo.h"
#include "Asset.h"
#include "var_alea.h"
#include <boost/shared_ptr.hpp>
using namespace std;


int main()
{
	//TODO debugg pour piger pourquoi j'ai tout le temps zero


	/*
	boost::shared_ptr<Cdo> toto();
	cout << "Hello World!" << endl;   cout << "Welcome to C++ Programming" << endl;
	char pause;
	cin >> pause;
	*/

	init_alea();
	//double alpha = 1;
	//double beta = 1;
	//double mu = 1;
	//double delta = 1;
	//normal_inverse_gaussian normal_inverse_gaussian(alpha, beta,
	//	mu, delta);

	int nbAssets = 125;
	double rate = 0.01;
	double K1 = 0, K2 = 0.1;
	double lambda = 0.05;//default intensity
	std::vector<double> spreadPaimentDates{ 0.5, 1, 1.5, 2, 2.5, 3 };

	std::vector<Asset> assets;
	for (int iAsset = 0; iAsset < nbAssets; iAsset++)
	{
		std::vector<double> defaultProbas;
		for (auto date : spreadPaimentDates)
		{
			defaultProbas.push_back(1-exp(-lambda*date));
		}
		Asset asset(0.3, ((double)1)/nbAssets, defaultProbas);
		assets.push_back(asset);
	}

	Cdo myCdo(K1, K2, spreadPaimentDates, assets);

	gaussian gaussianGen;

	//TODO sound strange time is not used, check with Lemaire M(t) X(t) : MB, N(0,1), N(0,t) ?
	int iDate = 0;
	std::vector<bool> hasDefaulted(myCdo.getAssets().size(),false);
	std::vector<double> lossInCDOByDate(myCdo.getSpreadPaimentDates());
	double defaultedPortion = 0;
	for (auto date : spreadPaimentDates)
	{	
		int iAsset = 0;
		for (auto asset : myCdo.getAssets())
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
		lossInCDOByDate[iDate] = myCdo.computeLossInCdo(defaultedPortion);
		iDate++;
	}
	double spread = myCdo.computeSpread(lossInCDOByDate, rate);

	cout << "spread: " << spread << endl;
	char pause;
	cin >> pause;
	return 0;
}

//TODO
//-Recovery rate