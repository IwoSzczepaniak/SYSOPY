#include <stdlib.h>
#include <stdio.h>
#include "bibl1.h"

/*napisz biblioteke ladowana dynamicznie przez program zawierajaca funkcje:

1) zliczajaca sume n elementow tablicy tab:
int sumuj(int *tab, int n);

2) liczaca srednia n elementow tablicy tab
double srednia(int *tab, int n);

*/

int sumuj(int *tab, int n)
{
    int res = 0;
    for(int i = 0; i<n; i++)
    {
        res += *tab+i;
    }
    return res;
}

extern double srednia(int *tab, int n)
{
    int res = 0;
    double ress;
    for(int i = 0; i<n; i++)
    {
        res += *tab+i;
    }
    ress = res/n;
    return ress;
}
