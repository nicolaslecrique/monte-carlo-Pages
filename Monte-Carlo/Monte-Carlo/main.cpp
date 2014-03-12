#include <iostream>
#include "Cdo.h"
#include "Asset.h"
#include "var_alea.h"
#include <boost/shared_ptr.hpp>
using namespace std;


int main()
{
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


	std::vector<double> spreadPaimentDates{ 0.5, 1, 1.5, 2, 2.5, 3 };

	std::vector<Asset> assets;
	for (int iAsset = 0; iAsset < 10; iAsset++)
	{
		std::vector<double> defaultProbas;
		for (int iDate = 0; iDate < (int)spreadPaimentDates.size(); iDate++)
		{
			defaultProbas.push_back(iDate*0.001);
		}
		Asset asset(iAsset*0.05, defaultProbas);
		assets.push_back(asset);
	}

	Cdo myCdo(0.1, 0.3, spreadPaimentDates, assets);

	gaussian gaussianGen;

	//TODO sound strange time is not used, check with Lemaire M(t) X(t) : MB, N(0,1), N(0,t) ?
	int iDate = 0;
	
	for (auto date : spreadPaimentDates)
	{
		for (auto asset : myCdo.getAssets())
		{
			double x = gaussianGen();
			double m = gaussianGen();
			asset.hasDefaulted(x, m, iDate);
		}
		iDate++;
	}
	//myCdo.computeLoss

	double spread = 0;// myCdo.computeSpread(/*TODO*/, 0.02);


	cout << "spread" << spread << "nig " << endl;
	char pause;
	cin >> pause;
	return 0;
}

//TODO
//-Recovery rate