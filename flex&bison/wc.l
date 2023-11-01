%option noyywrap

%{
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* Declarations and option settings. */
uint64_t char_count = 0;
uint64_t word_count = 0;
uint64_t line_count = 0;
%}

/* Patterns and actions. */
%%

[^ \t\n\r\f\v]+ { ++word_count; char_count += strlen(yytext); }
\n				{ ++char_count; ++line_count; }
.				{ ++char_count; }

%%

    /* C code section */
int main(int argc, char *argv[]) {
  // Handle stdin
  if (argc == 1) {
    yylex();
    printf("%llu %llu %llu\n", line_count, word_count, char_count);
    return 0;
  }

  uint64_t total_char_count = 0;
  uint64_t total_word_count = 0;
  uint64_t total_line_count = 0;

  for (int i = 1; i < argc; ++i) {
    FILE *f = fopen(argv[i], "r");
    if (!f) {
      perror(argv[1]);
      return 1;
    }

    yyrestart(f);
    yylex();
    fclose(f);

    printf("%llu %llu %llu %s\n", line_count, word_count, char_count, argv[i]);

    total_char_count += char_count;
    total_word_count += word_count;
    total_line_count += line_count;
    char_count = word_count = line_count = 0;
  }

  if (argc > 2)
    printf("%llu %llu %llu total\n", total_line_count, total_word_count,
           total_char_count);

  return 0;
}
