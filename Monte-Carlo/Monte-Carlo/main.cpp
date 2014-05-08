#include "TestsPricing.h"
#include "TestsQMC.h"
#include "TestNIG.h"

int main()
{

	init_alea();
	PrimeNumbersGenerator::fnGeneratePrimeList();

	//TestByNbSimu();
	//TestKakutaniByNbSimu();
	//PrimeNumbersGenerator::getNextPrimeNumber(true);
	//TestHaltonByNbSimu();
	//PrimeNumbersGenerator::getNextPrimeNumber(true);
	//TestKakutaniOneDimensionNbSimu();
	//TestByRecoveryRate();
	// TestByTranche();
	// TestByNbAssets();
	// TestByDefaultIntensity();
	// TestBycorrelatedCoeff();
	// TestByNbSimuWithoutRecovery();
	//TestQmcSequences();
	TestNIGInverseCumulative();





	return 0;
}

