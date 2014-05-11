#include <vector>
#include <functional>

//Compute the integral of a smooth function by simpson algorithm
//caches values at discretisation points then make a linear interpolation between
template<typename F>
class SimpsonIntegral
{
	std::vector<double> _result;
	double _min;
	double _step;

public:

	SimpsonIntegral(double min, double max, double step, F fct) :
	_min(min), _step(step)
	{
		double coeff = step/6;
		double halfStep = step/2;

		double f_next = 0;
		double currentIntegral = 0;

		for ( double x = min ; x<= max ; x+=step)
		{
			double f_current = f_next;
			double f_middle = fct(x-halfStep);
			f_next = fct(x);

			currentIntegral += coeff*(f_current+f_next+4*f_middle);
			_result.push_back(currentIntegral);
		}
	}

	//make linear interpolation between pre-computed points
	double operator()(const double x) const
	{
		//compute indexes around from x
		double index = (x-_min)/_step;
		if ( index <= 0)
		{
			return _result[0];
		}
		if ( index >= _result.size()-1)
		{
			return _result[_result.size()-1];
		}
		//linear interpolation
		int beforeIndex = (int)index;
		return _result[beforeIndex]*(1-index+beforeIndex)+_result[beforeIndex+1]*(index-beforeIndex);
	}
};
