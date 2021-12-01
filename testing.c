#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union
{
    int a;
    int b;
} testUnion;

void
dec2hex (int x)
{
    if (!x)
	{
	    putchar ('\n');
	    return;
	}

    dec2hex (x / 16);
    /** putchar ("0123456789ABCDEF"[x % 16]); */
}

int
get_a ()
{
    int c = 0;
    int d = 10;
    int e = 15;

    return c > 1 ? d > 2 || e > 5 ? 10 : 20 : 30;
}

#ifndef ID
#define ID 45871542
#endif

void
nextid ()
{
    int nextID = ID + 1;
    /** printf ("%d\n", nextID); */
}

int
main ()
{
    /** char s1[100]; */
    /** char s2[100]; */

    testUnion.a = 7;
    testUnion.b = 12;

    /** printf ("%d\n", testUnion.a + testUnion.b); */

    dec2hex (1234);
    int a = get_a ();
    assert (a == 30);

    signed int x = -10;
    bool _1 = (x >= 0L && ~x >= 0L);
    bool _2 = (x >= 0UL && ~x >= 0UL);
    bool _3 = (-x > 0);
    bool _4 = (x > 0 && ~x < x);
    bool _5 = (~x == x);

    unsigned int y = 100;
    bool __1 = (y >= 0L && ~y >= 0L);
    bool __2 = (y >= 0UL && ~y >= 0UL);
    bool __3 = (-y > 0);
    bool __4 = (y > 0 && ~y < y);
    bool __5 = (~y == y);

    /** assert (_1 == __1); */
    assert (_2 == __2);
    assert (_3 == __3);
    assert (_4 == __4);
    assert (_5 == __5);

    /** register int reg; */
    /** int d = &reg; */

    nextid ();

    unsigned num;
    num = num & ~num;
    /** printf ("num: %u\n", num); */

    char s1[100];
    char s2[100];

    /** gets (s1); */
    /** fgets (s2, sizeof (s2), stdin); */
    /**  */
    /** printf ("%d\n", strlen (s1) - strlen (s2)); */
    // -1

    char var1[10];
    char var2[5] = "Hello";

    strcpy (var1, var2);
    /** printf ("%s %s\n", var1, var2); */

    int p = 3;
    /** int *pp = &p; */

    /*
     * The question related to apple is the following:
     * Given the expression `sizeof (int) * p`, is `*` denoting a multiplaction
     * or ptr dereferencing
     * Answer: It is multiplication because of operator precedence
     */
    int apple = sizeof (int) * p;
    /** printf ("apple = %d\n", apple); */

    /*
     * The question related to zzz is the following
     *Given the expression `z = y+++x`, is that denoting y++ or ++x

Answer: y++
     */

    int yy = 0;
    int xx = 0;
    int zzz = yy++ + xx;

    int zzzz = yy++ + ++x;

    return 0;
}
