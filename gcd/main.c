#include <stdio.h>
#include <stdlib.h>

void
swap (int *pa, int *pb)
{
    int t = *pa;
    *pa = *pb;
    *pb = t;
}

int
gcd (int a, int b)
{
    if (a < b)
	swap (&a, &b);

    while (a)
	a %= b;

    return b;
}

int
main (int argc, char *argv[])
{
    if (argc != 3)
	return 1;

    int a = atoi (argv[1]), b = atoi (argv[2]);
    int __gcd = gcd (a, b);

    printf ("%d\n", __gcd);

    return 0;
}
