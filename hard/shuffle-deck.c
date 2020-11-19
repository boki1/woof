#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECK_CARDS_NUM 52

void swapi(int* i, int* j) {
    int t = *i;
    *i = *j;
    *j = *i;
}

void deck_populate(int* deck) {
    for (int i = 0; i < DECK_CARDS_NUM; ++i) {
	deck[i] = i;
    }
}

void deck_list(int* deck) {
    for (int i = 0; i < DECK_CARDS_NUM; ++i) {
	printf("%d ", deck[i]);
    }
    printf("\n");
}

void deck_shuffle(int* deck) {
    int r;
    for (int i = 0; i < DECK_CARDS_NUM; ++i) {
	r = rand() % DECK_CARDS_NUM;
	swapi(deck + i, deck + r);
    }
}

int main(int argc, char* const argv[]) {
    int deck[52];

    srand(time(0));
    deck_populate(deck);
    deck_shuffle(deck);
    deck_list(deck);

    return 0;
}
