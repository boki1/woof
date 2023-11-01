%option nodefault
%option noyywrap

/*
 * %x marks IFILE as an exclusive start state. This means that when this state
 * is entered only patterns specifically marked with this state can match.
 */
%x IFILE

%{
	struct bufstack {
		struct bufstack *prev;
		int lineno;
		const char *filename;
		FILE *file;
	} *curbs = NULL;

	const char *curfilename = NULL;

	int newfile(const char *filename);
	int popfile(void);
%}

%%

^"#"[ \t]*include[ \t]*[\"<]	{ BEGIN IFILE; }

<IFILE>[^ \t\n">]+ 	{
						int c;
						while ((c = input()) && c != '\n') ;
						++yylineno;
						printf("DEBUG: Go into %s\n", yytext);
						if (!newfile(yytext))
							yyterminate();
						BEGIN INITIAL;
					}

<IFILE>.|\n			{
						fprintf(stderr, "%4d bad include line\n", yylineno);
						yyterminate();
					}

<<EOF>>				{
						printf("DEBUG: Go out of %s\n", curfilename);
						if (!popfile())
							yyterminate();
					}

^.					{ fprintf(yyout, "%4d %s", yylineno, yytext); }
^\n					{ fprintf(yyout, "%4d %s", yylineno++, yytext); }
\n					{ ECHO; ++yylineno; }
.					{ ECHO; }

%%

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "need filename\n");
		return 1;
	}
	if (newfile(argv[1]))
		yylex();
	return 0;
}

int newfile(const char *filename) {
	FILE *f = fopen(filename, "r");
	if (!f) {
		perror(filename);
		return 0;
	}

	struct bufstack *bs = malloc(sizeof(struct bufstack));
	if (!bs) {
		perror("malloc");
		exit(1); // This is UB, right?
	}

	if (curbs)
		curbs->lineno = yylineno;

	bs->prev = curbs;
	bs->filename = filename;
	bs->file = f;
	bs->lineno = 1;

	yypush_buffer_state(yy_create_buffer(f, YY_BUF_SIZE));
	yylineno = 1;
	curbs = bs;
	curfilename = filename;

	return 1;
}

int popfile(void) {
	if (!curbs)
		return 0;

	struct bufstack *bs = curbs;
	fclose(curbs->file);

	struct bufstack *prevbs = curbs->prev;
	free(curbs);

	if (!prevbs)
		return 0;

	yypop_buffer_state();
	curbs = prevbs;
	yylineno = curbs->lineno;
	curfilename = curbs->filename;
	return 1;
}
