/*
 * Analyseur lexical du compilateur L en FLEX
 */ 
%{
/* code copié AU DÉBUT de l'analyseur */

#include "analyseur_syntaxique.tab.h"
#include "syntabs.h"
%}
%option yylineno
%option nounput
%option noinput

/* Déclarations à compléter ... */
                    
id  [a-zA-Z_$][a-zA-Z0-9_$]*      
                    
%%
[0-9]+ {yylval.ival=atoi(strdup(yytext));return NOMBRE;}
\; {yylval.sval=yytext;return POINT_VIRGULE;}
\+ {yylval.sval=yytext;return PLUS;}
\- {yylval.sval=yytext;return MOINS;}
\* {yylval.sval=yytext;return FOIS;}
\/ {yylval.sval=yytext;return DIVISE;}
\( {yylval.sval=yytext;return PARENTHESE_OUVRANTE;}
\) {yylval.sval=yytext;return PARENTHESE_FERMANTE;}
\[ {yylval.sval=yytext;return CROCHET_OUVRANT;}
\] {yylval.sval=yytext;return CROCHET_FERMANT;}
\{ {yylval.sval=yytext;return ACCOLADE_OUVRANTE;}
\} {yylval.sval=yytext;return ACCOLADE_FERMANTE;}
\= {yylval.sval=yytext;return EGAL;}
\< {yylval.sval=yytext;return INFERIEUR;}
\& {yylval.sval=yytext;return ET;}
\| {yylval.sval=yytext;return OU;}
\! {yylval.sval=yytext;return NON;}
"si" {yylval.sval=strdup(yytext);return SI;}
"alors" {yylval.sval=strdup(yytext);return ALORS;}
"sinon" {yylval.sval=strdup(yytext);return SINON;}
"tantque" {yylval.sval=strdup(yytext);return TANTQUE;}
"faire" {yylval.sval=strdup(yytext); return FAIRE;}
"entier" {yylval.sval=yytext; return ENTIER;}
"retour" {yylval.sval=strdup(yytext);return RETOUR;}
"lire" {yylval.sval=strdup(yytext);return LIRE;}
"ecrire" {yylval.sval=strdup(yytext);return ECRIRE;}
{id} {yylval.sval=strdup(yytext);return IDENTIF;}
\, {return VIRGULE;}
\n { }
[ \t] ;
#.* {};


%%

/* Code copié À LA FIN de l'analyseur */

int yywrap(){
  return 1;
}

/***********************************************************************
 * Fonction auxiliaire appelée par l'analyseur syntaxique pour 
 * afficher des messages d'erreur et l'arbre XML 
 **********************************************************************/

char *tableMotsClefs[] = {"si", "alors", "sinon", "tantque", "faire", "entier", "retour", "lire", "ecrire"};
int codeMotClefs[] = {SI, ALORS, SINON, TANTQUE, FAIRE, ENTIER, RETOUR, LIRE, ECRIRE};
int nbMotsClefs = 9;

void nom_token( int token, char *nom, char *valeur ) {
  int i;    
  strcpy( nom, "symbole" );
  if(token == POINT_VIRGULE) strcpy( valeur, "POINT_VIRGULE");
  else if(token == PLUS) strcpy(valeur, "PLUS");
  else if(token == MOINS) strcpy(valeur, "MOINS");
  else if(token == FOIS) strcpy(valeur, "FOIS");
  else if(token == DIVISE) strcpy(valeur, "DIVISE");
  else if(token == PARENTHESE_OUVRANTE) strcpy(valeur, "PARENTHESE_OUVRANTE");
  else if(token == PARENTHESE_FERMANTE) strcpy(valeur, "PARENTHESE_FERMANTE");
  else if(token == CROCHET_OUVRANT) strcpy(valeur, "CROCHET_OUVRANT");
  else if(token == CROCHET_FERMANT) strcpy(valeur, "CROCHET_FERMANT");
  else if(token == ACCOLADE_OUVRANTE) strcpy(valeur, "ACCOLADE_OUVRANTE");
  else if(token == ACCOLADE_FERMANTE) strcpy(valeur, "ACCOLADE_FERMANTE");
  else if(token == EGAL) strcpy(valeur, "EGAL");
  else if(token == INFERIEUR) strcpy(valeur, "INFERIEUR");
  else if(token == ET) strcpy(valeur, "ET");
  else if(token == OU) strcpy(valeur, "OU");
  else if(token == NON) strcpy(valeur, "NON");   
  else if(token == VIRGULE) strcpy(valeur, "VIRGULE"); 
  else if( token == IDENTIF ) {
    strcpy( nom, "identificateur" );    
    strcpy( valeur, yytext );    
  }
  else if( token == NOMBRE ) {
    strcpy( nom, "nombre" );
    strcpy( valeur, yytext ); 
  }
  else {
    strcpy(nom, "mot_clef");
    for(i = 0; i < nbMotsClefs; i++){
      if( token ==  codeMotClefs[i] ){
        strcpy( valeur, tableMotsClefs[i] );
        break;
      }
    }
  }  
}

