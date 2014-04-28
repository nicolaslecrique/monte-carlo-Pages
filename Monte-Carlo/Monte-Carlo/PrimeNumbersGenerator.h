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
#define MAX 100000000

char Prime[MAX];
int PrimeList[MAX/9], NumP;

/* Sieve of Eratosthenes: Generate all the prime numbers (< MAX) */
void fnGeneratePrimeList()
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