#include "TestsPricing.h"
#include "TestsQMC.h"
#include "TestNIG.h"

int main()
{

	init_alea();
	PrimeNumbersGenerator::fnGeneratePrimeList();

	TestByNbSimu();
	TestKakutaniByNbSimu();
	PrimeNumbersGenerator::getNextPrimeNumber(true);
	TestHaltonByNbSimu();
	PrimeNumbersGenerator::getNextPrimeNumber(true);
	TestKakutaniOneDimensionNbSimu();
	TestKakutaniCorrelationDim();
	TestByRecoveryRate();
	TestByTranche();
	TestByNbAssets();
	TestByDefaultIntensity();
	TestBycorrelatedCoeff(0.1);
	TestByNbSimuWithoutRecovery();
	TestQmcSequences();
	TestNIGInverseCumulative();
	TestNIGByAlpha(0.1);
	TestNIGByBeta(1);
	TestBycorrelatedCoeffAndTranche();
	TestNIGByAlphaAndTranches();
	TestNIGByBetaAlpha();



	return 0;
}

