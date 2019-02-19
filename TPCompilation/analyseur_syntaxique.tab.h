/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_ANALYSEUR_SYNTAXIQUE_TAB_H_INCLUDED
# define YY_YY_ANALYSEUR_SYNTAXIQUE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SI = 258,
    IDENTIF = 259,
    NOMBRE = 260,
    ALORS = 261,
    SINON = 262,
    TANTQUE = 263,
    FAIRE = 264,
    RETOUR = 265,
    OU = 266,
    ET = 267,
    EGAL = 268,
    INFERIEUR = 269,
    MOINS = 270,
    FOIS = 271,
    DIVISE = 272,
    NON = 273,
    PLUS = 274,
    PARENTHESE_OUVRANTE = 275,
    PARENTHESE_FERMANTE = 276,
    CROCHET_OUVRANT = 277,
    CROCHET_FERMANT = 278,
    ACCOLADE_OUVRANTE = 279,
    ACCOLADE_FERMANTE = 280,
    VIRGULE = 281,
    POINT_VIRGULE = 282,
    ENTIER = 283,
    MOINSU = 284
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "analyseur_syntaxique.y" /* yacc.c:1909  */
int ival; char *cval;
			struct n_l_instr_ *n_l_instr;
			struct n_instr_ *n_instr;
			struct n_exp_ *n_exp;
			struct n_l_exp_ *n_l_exp;
			struct n_var_ *n_var;
			struct n_l_dec_ *n_l_dec;
			struct n_dec_ *n_dec;
			struct n_prog_ *n_prog;
			struct n_appel_ *n_appel;
			;

#line 97 "analyseur_syntaxique.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ANALYSEUR_SYNTAXIQUE_TAB_H_INCLUDED  */
