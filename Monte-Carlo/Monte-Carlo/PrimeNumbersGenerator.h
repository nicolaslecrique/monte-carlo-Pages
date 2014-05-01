#pragma once



/************* Sieve of Eratoshenes for Prime Number Problem ***************
*                                                                          *
* Author: Peter Lee (peterlee.com.cn <at> gmail.com)                       *
*   Date: 2008-03-02                                                       *
*                                                                          *
* Please keep this information while referencing the code below.           *
* Thanks so much!                                                          *
*                                                                          *
* Welcome To Peter Lee's Blog Website:                                     *
*         http://www.peterlee.com.cn                                       *
*         http://blog.peterlee.com.cn                                      *
*                                                                          *
***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#define MAX 1000000



class PrimeNumbersGenerator
{

static char Prime[MAX];
static int PrimeList[MAX/9];
static int NumP;

public:
/* Sieve of Eratosthenes: Generate all the prime numbers (< MAX) */
static void fnGeneratePrimeList();

static int getNextPrimeNumber(bool reset = false);


};