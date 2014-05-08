#pragma once

#include <iostream>
#include <cmath>
#include <memory>
#include "NIGDistribution.h"
#include "GaussianDistribution.h"
#include <iostream>
#include <fstream> 

using namespace std;



void TestNIGInverseCumulative()
{
	std::ofstream outputFile;
  	outputFile.open ("test-NIG-inverse-cumulative.csv");
  	outputFile << "test;expected;result" << endl;

	double alpha;
	double beta;
	double mu;
	double delta;

	//Test 1 : inverse sould go to minus/plus infinity when proba go to 0/1
	alpha = 1;
	beta = 1;
	mu = 1;
	delta = 1;
	NIGDistribution nig1(alpha, beta, mu, delta);

	double inv1 = nig1.inverse_cumulative(0);
	outputFile << "proba->0;" << "-20;" << inv1 << endl;

	inv1 = nig1.inverse_cumulative(1);
	outputFile << "proba->1;" << "+20;" << inv1 << endl;

	//Test 2 : when beta=0 distribution is symetric centered on mu => inverse(0.5) should be = mu
	beta = 0;
	mu = 4.5;
	NIGDistribution nig2(alpha, beta, mu, delta);
	double inv2 = nig2.inverse_cumulative(0.5);
	outputFile << "beta=0-proba=0.5;" << "4.5;" << inv2 << endl;

	//Test 3 : when beta=0, delta=sigma2*alpha, alpha->infinity, law is the same as normal(mu, sigma2)
	mu = 0;
	beta = 0;
	alpha = 10;//alpha cannot ne greater because we do exp(alpha^2) in density computaion
	double sigma = 1;
	delta = sigma*sigma*alpha;
	GaussianDistribution gauss;
	NIGDistribution nig3(alpha, beta, mu, delta);
	for (double i = 0.1 ; i <= 0.91 ; i+=0.1)
	{
		double gaussInv = gauss.inverse_cumulative(i);
		double nigInv = nig3.inverse_cumulative(i);
		outputFile << "normal-vs-nig proba =" << i << ";" << gaussInv << ";" << nigInv << endl;
	}

  	outputFile.close();
}
