#include "Kakutani.h"


Kakutani::Kakutani(int base) : _base(base)
{
}


Kakutani::~Kakutani()
{
}

void Kakutani::NextStep(int x[], int y[])
{
	int size = (sizeof(x) / sizeof(*x));
	int remaining = 0;
	for (int i = 0; i < size; i++)
	{
		int temp = x[i] + y[i] + remaining;
		x[i] = temp%_base;
		remaining = temp / _base;
	}
	return;
}

