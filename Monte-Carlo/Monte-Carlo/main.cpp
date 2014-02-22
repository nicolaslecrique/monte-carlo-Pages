#include <iostream>
#include "Cdo.h"
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
	double alpha = 1;
	double beta = 1;
	double mu = 1;
	double delta = 1;
	normal_inverse_gaussian normal_inverse_gaussian(alpha, beta,
		mu, delta);

	Cdo myCdo(0.0, 0.2, std::vector<double>{0.5, 1, 1.5, 2, 2.5, 3});
	std::vector<double> expectedLosses{ 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };

	double spread = myCdo.computeSpread(expectedLosses, 0.02);
	cout << "spread" << spread << "nig " << normal_inverse_gaussian() << endl;
	char pause;
	cin >> pause;
	return 0;
}

//TODO
//-Recovery rate
