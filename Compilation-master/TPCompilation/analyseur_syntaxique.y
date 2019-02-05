%{
#include<stdlib.h>
#include<stdio.h>
#define YYDEBUG 1
//#include"syntabs.h" // pour syntaxe abstraite
//extern n_prog *n;   // pour syntaxe abstraite
extern FILE *yyin;    // declare dans compilo
extern int yylineno;  // declare dans analyseur lexical
int yylex();          // declare dans analyseur lexical
int yyerror(char *s); // declare ci-dessous
%}

%token SI
%token IDENTIF
%token NOMBRE
%token ALORS
%token SINON
%token TANTQUE
%token FAIRE
%token RETOUR
%token OU
%token ET
%token EGAL
%token INFERIEUR
%token MOINS
%token FOIS
%token DIVISE
%token NON
%token PLUS
%token PARENTHESE_OUVRANTE
%token PARENTHESE_FERMANTE
%token CROCHET_OUVRANT
%token CROCHET_FERMANT
%token ACCOLADE_OUVRANTE
%token ACCOLADE_FERMANTE
%token VIRGULE
%token POINT_VIRGULE
%token ENTIER

//...
//TODO: compléter avec la liste des terminaux
%start programme

%%

programme : ldecvaropt ldecfct ;
ldecvaropt : ldecvar POINT_VIRGULE | ;
ldecvar : decvar ldecvarbis ;
ldecvarbis : VIRGULE decvar ldecvarbis | ;
decvar : type IDENTIF  | type IDENTIF CROCHET_OUVRANT NOMBRE CROCHET_FERMANT ;
ldecfct : decfct | decfct ldecfct ;
decfct : IDENTIF PARENTHESE_OUVRANTE largopt PARENTHESE_FERMANTE ldecvaropt ibloc;
largopt : ldecvar | ;
ibloc : ACCOLADE_OUVRANTE linstr ACCOLADE_FERMANTE;
linstr : instr linstr | ;
instr : affectation | sialors | iappel | tantque | retour | ibloc | POINT_VIRGULE ;
affectation : IDENTIF EGAL expr POINT_VIRGULE ; 
sialors : SI expr ALORS ibloc sinonopt;
sinonopt : SINON ibloc | ;
tantque : TANTQUE expr FAIRE ibloc;
retour : RETOUR expr POINT_VIRGULE ;
iappel : appelfct POINT_VIRGULE ; 
appelfct : IDENTIF PARENTHESE_OUVRANTE lexpropt PARENTHESE_FERMANTE ;
lexpropt : lexpr | ;
lexpr : expr lexprbis;
lexprbis : VIRGULE expr lexprbis | ;
expr : expr OU expr1 | expr1;
expr1 : expr1 ET expr2 | expr2 ;
expr2 : expr2 EGAL expr3 | expr2 INFERIEUR expr3 | expr3;
expr3 : expr3 PLUS expr4 | expr3 MOINS expr4 | expr4;
expr4 : expr4 FOIS expr5 | expr4 DIVISE expr5 | expr5;
expr5 : NON expr5 | expr6;
expr6 : PARENTHESE_OUVRANTE expr6 PARENTHESE_FERMANTE | NOMBRE | appelfct | var;
var : IDENTIF | IDENTIF CROCHET_OUVRANT expr CROCHET_FERMANT ;
type : ENTIER ;


//TODO: compléter avec les productions de la grammaire

%%

int yyerror(char *s) {
  fprintf(stderr, "erreur de syntaxe ligne %d\n", yylineno);
  fprintf(stderr, "%s\n", s);
  fclose(yyin);
  exit(1);
}
