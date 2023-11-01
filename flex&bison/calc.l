%option noyywrap
%option nodefault

%{
#include <stdlib.h>

#include "calc.tab.h"

extern unsigned hex2dec(unsigned);
extern unsigned bin2dec(unsigned);
extern unsigned oct2dec(unsigned);

%}

%%
"//".*
"+"				{ return ADD; }
"-"				{ return SUB; }
"*"				{ return MUL; }
"/"				{ return DIV; }
"%"				{ return MOD; }
"|"				{ return ABS; }
"("				{ return PAREN_LEFT; }
")"				{ return PAREN_RIGHT; }
[1-9][0-9]*  	{ yylval = atoi(yytext); 							return NUMBER; }
0x[0-9a-fA-F]+ 	{ yylval = hex2dec(strtol(yytext, NULL, 16)); 		return NUMBER; }
0b[01]+ 		{ yylval = bin2dec(strtol(yytext + 2, NULL, 2)); 	return NUMBER; }
0[0-7]* 		{ yylval = oct2dec(strtol(yytext, NULL, 8)); 		return NUMBER; }
\n 	    		{ return EOL; }
[ \t]
.				{ printf("Mistery character %c\n", *yytext); }
%%
