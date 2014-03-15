#include <iostream>
#include <cmath>
#include "Cdo.h"
#include "Asset.h"
#include "MonteCarloCdoEngine.h"
#include "var_alea.h"

using namespace std;


int main()
{

	init_alea();

	int nbAssets = 125;
	double rate = 0.01;
	double K1 = 0.2, K2 = 0.3;
	double lambda = 0.01;//default intensity
	std::vector<double> spreadPaimentDates{ 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5 };

	std::vector<Asset> assets;
	for (int iAsset = 0; iAsset < nbAssets; iAsset++)
	{
		std::vector<double> defaultProbas;
		for (auto date : spreadPaimentDates)
		{
			defaultProbas.push_back(1-exp(-lambda*date));
		}
		Asset asset(0.5, ((double)1)/nbAssets, defaultProbas);
		assets.push_back(asset);
	}

	Cdo myCdo(K1, K2, spreadPaimentDates, assets);
	MonteCarloCdoEngine engine;

	auto result = engine.Price(myCdo, 1000, rate);

	cout << "spread: " << result.Spread << endl;
	char pause;
	cin >> pause;
	return 0;
}

//TODO
//-Recovery rate