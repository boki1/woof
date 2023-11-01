
all: wc calc incl

wc: wc.l
	flex wc.l
	gcc lex.yy.c -lfl -o wc

incl: incl.l
	flex incl.l
	gcc lex.yy.c -lfl -o incl

calc: calc.y calc.l
	bison -d calc.y
	flex calc.l
	gcc calc.tab.c lex.yy.c -lfl -o calc

clean:
	rm -f lex.yy.c *.tab.h *.tab.c *.output calc wc incl

.PHONY: all clean
