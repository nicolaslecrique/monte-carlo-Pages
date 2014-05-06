#pragma once

//usefull with windows compiler for pi
#define _USE_MATH_DEFINES

#include <cmath>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/math/tools/minima.hpp>
#include "Distribution.h"
#include "SimpsonIntegral.h"


class NIGDistribution : public Distribution
{
	class densityFct : public std::unary_function<double,double>
	{
		double _alpha;
		double _beta;
		double _mu;
		double _delta;
	public:
		densityFct(double alpha, double beta, double mu, double delta)
		 : _alpha(alpha), _beta(beta), _mu(mu), _delta(delta){}

		double operator()(double x) const
		{
			double gamma = sqrt(_alpha*_alpha - _beta*_beta);
			double K1 = boost::math::cyl_bessel_k(1.,_alpha*sqrt(_delta*_delta+(x-_mu)*(x-_mu)));
			return K1*_delta*_alpha*exp(_delta*gamma+_beta*(x-_mu))/(M_PI*sqrt(_delta*_delta + (x-_mu)*(x-_mu)));
		}
	};

	const SimpsonIntegral<densityFct> _cumulative;


public:
	NIGDistribution(double alpha, double beta, double mu, double delta)
	: _cumulative(-10, 10, 0.01, densityFct(alpha, beta, mu, delta))
	{
	}

	double inverse_cumulative(double proba) const
	{
		struct MinCumulative
		{
			double _proba;
			const SimpsonIntegral<densityFct>& _cumulative;


			MinCumulative(double proba, const SimpsonIntegral<densityFct>& cumulative) : _proba(proba), _cumulative(cumulative) {}

			double operator()(double x) const
			{
				return fabs(_cumulative(x)-_proba);
			}
		};

    	std::pair<double, double> result = boost::math::tools::brent_find_minima(MinCumulative(proba, _cumulative), -10.0, 10.0, 20);
    	return result.first;

	}

};

