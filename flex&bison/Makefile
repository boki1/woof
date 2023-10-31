
all: wc calc

wc: wc.l
	flex wc.l
	gcc lex.yy.c -lfl -o wc

calc: calc.y calc.l
	bison -d calc.y
	flex calc.l
	gcc calc.tab.c lex.yy.c -lfl -o calc

clean:
	rm -f lex.yy.c *.tab.h *.tab.c *.output calc wc

.PHONY: all clean
