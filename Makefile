YACC = bison -d -t 
CC = gcc
FLEX = flex

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical_flex.o util.o analyseur_syntaxique.tab.o 

all: compilo

compilo: compilo.c $(OBJ)
	$(CC) $(CCFLAGS) -o compilo compilo.c $(OBJ)

analyseur_syntaxique.tab.c: analyseur_syntaxique.y 
	$(YACC) $<

analyseur_lexical_flex.c: analyseur_lexical.flex analyseur_syntaxique.tab.c analyseur_syntaxique.tab.h
	$(FLEX) -o $@ $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f compilo
	- rm -f test_yylex
	- rm -f analyseur_lexical_flex.c
	- rm -f analyseur_syntaxique.tab.c
