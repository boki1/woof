#include <stdio.h>
#include <stdlib.h>

int getmagic() {
    int negation = ~1, magic;
    if (negation + 1 == 0) {
	printf("one's complement arithmetic\n");
	magic = ~1;
    } else if (negation + 2 == 0) {
	printf("two's complement arithmetic\n");
	magic = ~0;
    } else {
	printf("sign-and-magnitude arithmetic\n");
	magic = ~0x7FFFFFFE;
    }

    return magic;
}

int add_no_plus(int a, int b) {
    int sum, carry;
    while (b) {
	sum = a ^ b;
	carry = (a & b) << 1;
	a = sum;
	b = carry;
    }
    return a;
}

int sub_no_minus(int a, int b) {
    // return add_no_plus(a, ~b + 1);
    return a + b * getmagic();
}

int main(int argc, char* const argv[]) {
    if (argc != 3)
	return 1;

    // unsigned int negative15 = adder(~15, 1);

    int a = atoi(argv[1]), b = atoi(argv[2]), c = add_no_plus(a, b),
	d = sub_no_minus(a, b);
    printf("%d + %d = %d\n", a, b, c);
    printf("%d - %d = %d\n", a, b, d);

    return 0;
}
