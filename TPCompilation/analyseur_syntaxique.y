%{
#include<stdlib.h>
#include<stdio.h>
#define YYDEBUG 1
#include "syntabs.h" 
#include "affiche_arbre_abstrait.h"

n_prog *n;   // pour syntaxe abstraite
extern FILE *yyin;    // declare dans compilo
extern int yylineno;  // declare dans analyseur lexical
int yylex();          // declare dans analyseur lexical
int yyerror(char *s); // declare ci-dessous
void affiche_n_prog(n_prog *n);
%}
%union {int ival; char *cval;
			struct n_l_instr_ *n_l_instr;
			struct n_instr_ *n_instr;
			struct n_exp_ *n_exp;
			struct n_l_exp_ *n_l_exp;
			struct n_var_ *n_var;
			struct n_l_dec_ *n_l_dec;
			struct n_dec_ *n_dec;
			struct n_prog_ *n_prog;
			struct n_appel_ *n_appel;
			;}
%token SI
%token <cval> IDENTIF
%token <ival> NOMBRE
%token ALORS
%token SINON
%token TANTQUE
%token FAIRE
%token RETOUR
%token  OU
%token  ET
%token  EGAL
%token  INFERIEUR
%token  MOINS
%token  FOIS
%token  DIVISE
%token  NON
%token  PLUS
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
%type <n_instr> iappel
%type <n_appel> appelfct

%type <n_l_exp> lexpr
%type <n_l_exp> lexprbis
%type <n_exp> expr

%type <n_var> var




%left OU
%left ET
%left EGAL
%left INFERIEUR
%left PLUS MOINS
%left FOIS DIVISE
%left NON
%left NOMBRE
%nonassoc MOINSU



//...
//TODO: compléter avec la liste des terminaux
%start programme

%%

programme : ldecvaropt ldecfct {  $$ = cree_n_prog($1,$2);n=$$; };
ldecvaropt : ldecvar POINT_VIRGULE { $$ = cree_n_l_dec(NULL,$1); } | {$$ = NULL ;};
ldecvar : decvar ldecvarbis { $$ = cree_n_l_dec($1,$2); } ;
ldecvarbis : VIRGULE decvar ldecvarbis { $$ = cree_n_l_dec($2,$3); } | { $$ = NULL; };
decvar : ENTIER IDENTIF { $$ = cree_n_dec_var($2); } | ENTIER IDENTIF CROCHET_OUVRANT NOMBRE CROCHET_FERMANT { $$ = cree_n_dec_tab($2,$4); };
ldecfct : decfct { $$ = cree_n_l_dec($1,NULL); }| decfct ldecfct { $$ = cree_n_l_dec($1,$2); } ;
decfct : IDENTIF PARENTHESE_OUVRANTE largopt PARENTHESE_FERMANTE ldecvaropt ibloc { $$ = cree_n_dec_fonc($1,$3,$5,$6); };
largopt : ldecvar { $$ = cree_n_l_dec(NULL,$1); } | { $$ = NULL; };
ibloc : ACCOLADE_OUVRANTE linstr ACCOLADE_FERMANTE { $$ = cree_n_instr_bloc($2); };
linstr : instr linstr { $$ = cree_n_l_instr($1,$2); } | { $$ = NULL; } ;
instr : affectation  | sialors | iappel | tantque | retour ;
affectation : var EGAL expr POINT_VIRGULE {$$ = cree_n_instr_affect($1,$3);  };
sialors : SI expr ALORS ibloc sinonopt { $$ = cree_n_instr_si($2,$4,$5); };
sinonopt : SINON ibloc { $$ = cree_n_instr_bloc(cree_n_l_instr($2,NULL));} | { $$ = NULL; } ;
tantque : TANTQUE expr FAIRE ibloc { $$ = cree_n_instr_tantque($2,$4); };
retour : RETOUR expr POINT_VIRGULE { $$ = cree_n_instr_retour($2); } ;
iappel : appelfct POINT_VIRGULE { $$ = cree_n_instr_appel($1); } ; 
appelfct : IDENTIF PARENTHESE_OUVRANTE lexpr PARENTHESE_FERMANTE { $$ = cree_n_appel($1,$3); } ;
lexpr : expr lexprbis { $$ = cree_n_l_exp($1,$2);}| {$$ = NULL;} ;
lexprbis : VIRGULE expr lexprbis {$$ = cree_n_l_exp($2,$3); }  | { $$ = NULL; } ;
expr : expr OU expr { $$ = cree_n_exp_op(ou,$1,$3); } 
 | expr ET expr { $$ = cree_n_exp_op(et,$1,$3); }
 | expr EGAL expr {$$ = cree_n_exp_op(egal,$1,$3);}
 | expr INFERIEUR expr {$$ = cree_n_exp_op(inferieur,$1,$3);printf("lul");}
 | expr PLUS expr {$$ = cree_n_exp_op(plus,$1,$3);}
 | expr MOINS expr {$$ = cree_n_exp_op(moins,$1,$3);}
 | expr FOIS expr {$$ = cree_n_exp_op(fois,$1,$3);}
 | expr DIVISE expr {$$ = cree_n_exp_op(divise,$1,$3);}
 | NON expr {$$ = cree_n_exp_op(non,$2,NULL);}
 | PARENTHESE_OUVRANTE expr PARENTHESE_FERMANTE {$$=$2;}
 | MOINS expr %prec MOINSU {$$ = cree_n_exp_op(moins,$2,NULL);}
 | NOMBRE {$$ = cree_n_exp_entier($1);}
 | appelfct {$$ = cree_n_exp_appel($1);}
 | var {$$ = cree_n_exp_var($1);}

var : IDENTIF { $$ = cree_n_var_simple($1); } | IDENTIF CROCHET_OUVRANT expr CROCHET_FERMANT { $$ = cree_n_var_indicee($1,$3); } ;




%%



int yyerror(char *s) {
  fprintf(stderr, "erreur de syntaxe ligne %d\n", yylineno);
  fprintf(stderr, "%s\n", s);
  fclose(yyin);
  exit(1);
}




