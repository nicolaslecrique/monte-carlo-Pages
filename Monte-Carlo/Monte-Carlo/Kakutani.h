#pragma once
class Kakutani
{
public:
	Kakutani(int base);
	~Kakutani();

private:
	const int _base;
	void NextStep(int x[], int y[]);

};

