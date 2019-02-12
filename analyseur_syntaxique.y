%{
#include<stdlib.h>
#include<stdio.h>
#define YYDEBUG 1
#include"syntabs.h" // pour syntaxe abstraite

extern n_prog *n;   // pour syntaxe abstraite
extern FILE *yyin;    // declare dans compilo
extern int yylineno;  // declare dans analyseur lexical
int yylex();          // declare dans analyseur lexical
int yyerror(char *s); // declare ci-dessous
%}
%union {int ival; char cval;
			n_l_instr_ n_l_instr;
			n_instr_ n_instr;
			n_exp_ n_exp;
			n_l_exp_ n_l_exp;
			n_var_ n_var;
			n_l_dec_ n_l_dec;
			n_dec_ n_dec;
			n_prog_ n_prog;
			n_appel_ n_appel;}
%token SI
%token <cval> IDENTIF
%token <ival> NOMBRE
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

%type <n_prog> programme
%type <n_l_dec> ldecvaropt
%type <n_l_dec> ldecfct
%type <n_l_dec> ldecvar
%type <n_l_dec> ldecvarbis
%type <n_dec> decvar
%type <n_dec> decfct
%type <n_l_dec> largopt
%type <n_instr> ibloc
%type <n_l_instr> linstr
%type <n_instr> instr
%type <n_instr> affectation
%type <n_instr> sialors
%type <n_instr> sinonopt
%type <n_instr> tantque
%type <n_instr> retour
%type <n_appel> iappel
%type <n_appel> appelfct
%type <n_l_exp> lexpropt
%type <n_l_exp> lexpr
%type <n_l_exp> lexprbis
%type <n_exp> expr
%type <n_var> var
%type <> type


%left OU
%left ET
%left EGAL
%left INFERIEUR
%left PLUS MOINS
%left FOIS DIVISE
%left NON
%nonassoc MOINSU
//...
//TODO: compléter avec la liste des terminaux
%start programme

%%

programme : ldecvaropt ldecfct { $$ = cree_n_prog($1,$2); };
ldecvaropt : ldecvar POINT_VIRGULE | { $$ = NULL };
ldecvar : decvar ldecvarbis { $$ = cree_n_l_dec($1,$2); } ;
ldecvarbis : VIRGULE decvar ldecvarbis { $$ = cree_n_l_dec($2,$3); } | { $$ = NULL };
decvar : type IDENTIF { $$ = cree_n_dec_var($2); } | type IDENTIF CROCHET_OUVRANT NOMBRE CROCHET_FERMANT { $$ = cree_n_dec_tab($2,$4); };
ldecfct : decfct { $$ = cree_n_l_dec($1,NULL); }| decfct ldecfct { $$ = cree_n_l_dec($1,$2); };
decfct : IDENTIF PARENTHESE_OUVRANTE largopt PARENTHESE_FERMANTE ldecvaropt ibloc { $$ = cree_n_dec_fonc($1,$3,$5,$6); };
largopt : ldecvar | { $$ = NULL; };
ibloc : ACCOLADE_OUVRANTE linstr ACCOLADE_FERMANTE { $$ = cree_n_instr_bloc($2); };
linstr : instr linstr { $$ = cree_n_l_instr($1,$2); } | ;
instr : affectation | sialors | iappel | tantque | retour | ibloc | POINT_VIRGULE ;
affectation : var EGAL expr POINT_VIRGULE { $$ = cree_n_instr_affect($1,$3); }; 
sialors : SI expr ALORS ibloc sinonopt { $$ = cree_n_instr_si($2,$4,$5); };
sinonopt : SINON ibloc | ;
tantque : TANTQUE expr FAIRE ibloc;
retour : RETOUR expr POINT_VIRGULE ;
iappel : appelfct POINT_VIRGULE ; 
appelfct : IDENTIF PARENTHESE_OUVRANTE lexpropt PARENTHESE_FERMANTE ;
lexpropt : lexpr | ;
lexpr : expr lexprbis;
lexprbis : VIRGULE expr lexprbis | ;
expr : expr OU expr 
 | expr ET expr 
 | expr EGAL expr 
 | expr INFERIEUR expr
 | expr PLUS expr 
 | expr MOINS expr 
 | expr FOIS expr 
 | expr DIVISE expr 
 | NON expr 
 | PARENTHESE_OUVRANTE expr PARENTHESE_FERMANTE 
 | MOINS expr %prec MOINSU 
 | NOMBRE 
 | appelfct 
 | var;
var : IDENTIF { return cree_n_var_simple($1); } | IDENTIF CROCHET_OUVRANT expr CROCHET_FERMANT ;
type : ENTIER  ;


//TODO: compléter avec les productions de la grammaire

%%



int yyerror(char *s) {
  fprintf(stderr, "erreur de syntaxe ligne %d\n", yylineno);
  fprintf(stderr, "%s\n", s);
  fclose(yyin);
  exit(1);
}


