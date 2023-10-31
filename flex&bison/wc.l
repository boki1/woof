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

[a-zA-Z]+ 	{ ++word_count; char_count += strlen(yytext); }
\n			{ ++char_count; ++line_count; }
.			{ ++char_count; }

%%

/* C code section */
int main(int argc, char *argv[]) {
	yylex();
	printf("%llu %llu %llu\n", line_count, word_count, char_count);
	return 0;
}
