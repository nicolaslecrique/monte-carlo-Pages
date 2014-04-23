#pragma once

//usefull with windows compiler for pi
#define _USE_MATH_DEFINES

#include <cmath>
#include <boost/math/special_functions/bessel.hpp>
//#include <boost/numeric/odeint.hpp>
#include "Distribution.h"


class NIGDistribution : public Distribution
{
	double _alpha;
	double _beta;
	double _mu;
	double _delta;

public:
	NIGDistribution(double alpha, double beta, double mu, double delta) : _alpha(alpha), _beta(beta), _mu(mu), _delta(delta)
	{

	}

	double density(double x) const
	{
		double gamma = sqrt(_alpha*_alpha - _beta*_beta);
		double K1 = boost::math::cyl_bessel_k(1.,_alpha*sqrt(_delta*_delta+(x-_mu)*(x-_mu)));
		return K1*_delta*_alpha*exp(_delta*gamma+_beta*(x-_mu))/(M_PI*sqrt(_delta*_delta + (x-_mu)*(x-_mu)));
	}

	double cumulative(double x) const
	{
		//integrate( lorenz , x , 0.0 , 25.0 , 0.1 , write_lorenz );
	}

	double inverse_cumulative(double proba) const
	{
		//bisection sur la cumulative
	}

};

