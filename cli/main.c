#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

static const int CHUNK_SIZE = 128;

void
analyze_chunk(double *words, double *sentences, double *letters, char *buf) {
  assert(letters);
  assert(sentences);
  assert(buf);
  assert(buf[CHUNK_SIZE] == '\0');

  int spaces = 0;
  char *str = buf;
  while (str && (str = strchr(str, ' ')) != 0) {
	++spaces;
	str++;
  }

  str = buf;

  const char *sentence_separators[] = {".", "?", "!", "..."};
  int loc_sentences = 0;
  for (int i = 1; i < 4; ++i) {
	while (str && (str = strstr(str, sentence_separators[i])) != 0) {
	  loc_sentences++;
	  str += strlen(sentence_separators[i]);
	}
  }
  while (str && (str = strstr(str, sentence_separators[0])) != 0)
	if (str != buf + CHUNK_SIZE - 1
		&& *(str + 1) != '.') {
	  loc_sentences++;
	  str++;
	}

  int b_size = strlen(buf);
  int loc_letters = 0;
  for (int i = 0; i < b_size; ++i)
	if (isalnum(buf[i])) loc_letters++;

  *sentences += loc_sentences;
  *words += spaces;
  *letters += loc_letters;
}

int main
	(int argc, const char *const *argv) {
  if (argc == 1) {
	puts("insufficient number of arguments\n");
	return EXIT_FAILURE;
  }

  FILE *f;
  double words_total = 0, sentences_total = 0, letters_total = 0;
  char *const buffer = malloc(CHUNK_SIZE + sizeof "");
  for (int i = 1; i < argc; ++i) {
	f = fopen(argv[i], "r");
	if (f == NULL) {
	  printf("failed opening '%s'\n", argv[i]);
	  continue;
	}
	rewind(f);
	int _read;
	words_total = sentences_total = letters_total = 0;
	while ((_read = fread(buffer, sizeof(char), CHUNK_SIZE, f)) != 0) {
	  buffer[_read] = 0;
	  analyze_chunk(&words_total, &sentences_total, &letters_total, buffer);
//	  printf("W: %.0f\tS: %f.0\tL: %f.0\n", words_total, sentences_total, letters_total);
	}

	double L = letters_total / words_total * 100;
	double S = sentences_total / words_total * 100;
	double CLI = 0.0588 * L - 0.296 * S - 15.8;
	printf("%s: %.1f\n", argv[i], CLI);
  }

  return EXIT_SUCCESS;

}
