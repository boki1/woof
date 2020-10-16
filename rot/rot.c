#include <stdio.h>
#include <stdlib.h>

#include "rot.h"

uint32_t rotl(uint32_t num, unsigned cnt) {
    const unsigned bits = CHAR_BIT * sizeof(num) - 1;
    // assert cnt <= bits
    cnt &= bits;
    return (num << cnt) | (num >> (-cnt & bits));
}

uint32_t rotr(uint32_t num, unsigned cnt) {
    const unsigned bits = CHAR_BIT * sizeof(num) - 1;
    cnt &= bits;

    return (num >> cnt) | (num << (-cnt & bits));
}

int main(int argc, char* const argv[]) {
    uint32_t test_int = 0x8342;
    uint32_t test_int_rotr = rotr(test_int, 2);

    printf("%i\n", test_int_rotr);

    return EXIT_SUCCESS;
}
