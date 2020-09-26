#include <assert.h>
#include <stdio.h>

int count_ones(int num) {
    unsigned c;
    for (c = 0; num; ++c) {
	num &= (num - 1);
    }
    return c;
}
void tobinary(int num, char* buf, int buf_size) {
    for (int i = 0; i < buf_size; i++)
	buf[i] = (num & (int)1 << (buf_size - i - 1)) ? '1' : '0';
    buf[buf_size] = 0;
}

int main() {
    const int buf_size = 9;
    char buf[buf_size];

    unsigned int bset = 0;
    int l = 0;
    for (int i = 0; i < 100; ++i) {
	tobinary(i, buf, buf_size);
	bset = count_ones(i);
	printf("%d (10) = %s (2); %d bits set\n", i, buf, bset);
    }
    return 0;
}
