#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int type_fib;

struct fib {
    type_fib (*call)(type_fib, struct fib*);
    type_fib* mem;
};

type_fib fib(type_fib n, struct fib* mem) {
    if (n == 0)
	return n;
    if (!mem->mem[n])
	mem->mem[n] = fib(n - 1, mem) + fib(n - 2, mem);
    return mem->mem[n];
}

int main(int argc, char* const argv[]) {
    type_fib mem[100];
    for (int i = 0; i < 100; ++i)
	mem[i] = 0;
    mem[1] = 1;

    struct fib f = {.call = fib, .mem = mem};

    f.call(7, &f);

    for (int i = 0; i < 100; ++i) {
	printf("%d ", f.mem[i]);
    }
    printf("\n");

    return 0;
}
