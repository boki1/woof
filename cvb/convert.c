#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//
// TODO:
// Am I ran from the shell?
// If not show the output in 'dmenu'
const char* get_process_name_by_pid(const int pid) {
    char* name = (char*)calloc(1024, sizeof(char));
    if (name) {
	sprintf(name, "/proc/%d/cmdline", pid);
	FILE* f = fopen(name, "r");
	if (f) {
	    size_t size;
	    size = fread(name, sizeof(char), 1024, f);
	    if (size > 0) {
		if ('\n' == name[size - 1])
		    name[size - 1] = '\0';
	    }
	    fclose(f);
	}
    }
    return name;
}

#define DECODE_STR \
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define DECODE_STR_LENGTH 62

int d2v(char c) {
    int idx = 0;
    while (DECODE_STR[idx] != c && idx < DECODE_STR_LENGTH)
	++idx;
    return idx;
}

int s2n(char* s, int b) {
    int i = 0;
    int num = 0;
    while (s[i]) {
	num *= b;
	num += d2v(s[i]);
	++i;
    }
    return num;
}

int calcNumBaseTwoSize(int num, int base) {
    int sNumSize = 1;
    while (num >= base) {
	num /= base;
	sNumSize++;
    }
    return sNumSize;
}

void n2s(int num, int base, char* sNum, const int size) {
    int i = size - 1;
    int remainder;
    while (num >= base) {
	remainder = num % base;
	num /= base;
	sNum[i] = DECODE_STR[remainder];
	--i;
    }
    sNum[i] = DECODE_STR[num];
}

void help(void) {
    printf(
	"Use: bconv BASE_ONE BASE_TWO VALUE"
	"--help"
	"--max         %d",
	DECODE_STR_LENGTH);
}

void parse_args(int c, char** v) {
    if (c == 1) {
	printf("bad usage\n");
	help();
    }
}

int main(int argc, char* argv[]) {
    if (argc == 4) {
	int base1 = atoi(argv[1]);
	int base2 = atoi(argv[2]);
	char* sNumBaseOne = argv[3];

	// Convert sNumBaseOne to Decimal
	int Num;
	if (base1 != 10)
	    Num = s2n(sNumBaseOne, base1);
	else
	    Num = atoi(sNumBaseOne);

	const int NUM_BASE_TWO_SIZE = calcNumBaseTwoSize(Num, base2);
	char sNum[NUM_BASE_TWO_SIZE];

	// Convert Num from Decimal to sNumBaseTwo
	if (base2 != 10)
	    n2s(Num, base2, sNum, NUM_BASE_TWO_SIZE);
	else
	    sprintf(sNum, "%d", Num);

	for (int i = 0; i < NUM_BASE_TWO_SIZE; ++i)
	    printf("%c", sNum[i]);
	printf("\n");
    } else {
	parse_args(argc, argv);
    }
    return 0;
}
