#include "PrimeNumbersGenerator.h"


char PrimeNumbersGenerator::Prime[MAX];
int PrimeNumbersGenerator::PrimeList[MAX/9];
int PrimeNumbersGenerator::NumP;


void PrimeNumbersGenerator::fnGeneratePrimeList()
{
    int p, i;

    /* 0 and 1 are not prime numbers */
    memset(Prime+2, 1, sizeof(Prime)-2);
    p = 2; /* Smallest prime number */

    while(p*p < MAX)
    {
        /* Set multiples of p as non-prime (from p*p) */
        for(i = p*p; i < MAX; i += p)
            Prime[i] = 0;
        /* Move to next prime number, 
        i.e., the first number remaining in the list */
        while(!Prime[++p]);
    }
    /* Now, all the numbers remaining in the list are prime */
    for(i = 2; i <= MAX; i++)
        if(Prime[i])
            PrimeList[NumP++] = i;
}

int PrimeNumbersGenerator::getNextPrimeNumber(bool reset)
{
    static int currenIndex = 0;
    if (currenIndex == MAX/9 || reset)
    {
        currenIndex = 0;
    }
    return PrimeList[currenIndex++];
}
