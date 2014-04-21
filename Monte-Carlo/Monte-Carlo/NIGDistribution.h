#pragma once

//usefull with windows compiler for pi
#define _USE_MATH_DEFINES

#include <cmath>
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
		double K1;//TODO !!!!

		return K1*_delta*_alpha*exp(_delta*gamma+_beta*(x-_mu))/(M_PI*sqrt(_delta*_delta + (x-_mu)*(x-_mu)));
	}

	double cumulative(double x) const
	{
		//integration numérique
	}

	double inverse_cumulative(double proba) const
	{
		//bisection sur la cumulative
	}

};

