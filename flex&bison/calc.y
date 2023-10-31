%{
#include <stdio.h>
%}

%token NUMBER
%token ADD SUB MUL DIV ABS MOD
%token PAREN_LEFT PAREN_RIGHT
%token EOL

%%

statements: %empty
		| statements EOL
		| statements expr EOL { printf(" = %d\n", $2); }
		;

expr: factor
	| expr ADD factor 	{ $$ = $1 + $3; }
	| expr SUB factor 	{ $$ = $1 - $3; }
	;

factor: term
	  | factor MUL term	{ $$ = $1 * $3; }
	  | factor DIV term { $$ = $1 / $3; }
	  | factor MOD term { $$ = $1 % $3; }
	  ;

term: NUMBER
	| ABS term ABS { $$ = $2 >= 0 ? $2 : -$2; }
	| PAREN_LEFT expr PAREN_RIGHT { $$ = $2; }
	| SUB term { $$ = -$2; }
	;

%%

int main(int argc, char *argv[]) {
	yyparse();
	return 0;
}

int yyerror(char *s) {
	fprintf(stderr, "error: %s\n", s);
	return 0;
}

unsigned bpt2dec(unsigned x, size_t mask) {
	unsigned indec = 0;
    const size_t bits = __builtin_popcount(mask);
	for (size_t i = 0; x != 0; ++i, x >>= bits)
		indec += ((x & mask) << (i * bits));
	return indec;
}

unsigned hex2dec(unsigned x) {
    return bpt2dec(x, 0xf);
}

unsigned oct2dec(unsigned x) {
    return bpt2dec(x, 07);
}

unsigned bin2dec(unsigned x) {
    return bpt2dec(x, 0b1);
}
