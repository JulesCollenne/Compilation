#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syntabs.h"
#include "util.h"
#include "tabsymboles.h"
#include "parcours_arbre_abstrait.h"
#include "code3a.h"

extern int portee;
extern int adresseLocaleCourante;
extern int adresseArgumentCourant;
extern code3a_ code3a;
extern int trace_tabsymb;
int adresseGlobaleCourante = 0;
int nbE = 0;

/*-------------------------------------------------------------------------*/

void parcours_n_prog(n_prog *n)
{
	portee = P_VARIABLE_GLOBALE;
	code3a_init();
  parcours_l_dec(n->variables);
  parcours_l_dec(n->fonctions); 
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void parcours_l_instr(n_l_instr *n)
{
  if(n){
	  parcours_instr(n->tete);
	  parcours_l_instr(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr(n_instr *n)
{
  if(n){
    if(n->type == blocInst) parcours_l_instr(n->u.liste);
    else if(n->type == affecteInst) parcours_instr_affect(n);
    else if(n->type == siInst) parcours_instr_si(n);
    else if(n->type == tantqueInst) parcours_instr_tantque(n);
    else if(n->type == appelInst) parcours_instr_appel(n);
    else if(n->type == retourInst) parcours_instr_retour(n);
    else if(n->type == ecrireInst) parcours_instr_ecrire(n);
  }
}


char* intToString(int nb){
	char* s = malloc(sizeof(char)*(nb/10));
	sprintf(s,"%d",nb);
	return s;
}
/*-------------------------------------------------------------------------*/

void parcours_instr_si(n_instr *n)
{  
  operande *e1 = code3a_new_etiquette(getE());
  
  operande *zero = code3a_new_constante(0);
  operande *e2 = parcours_exp(n->u.si_.test);
  operande *e3 = code3a_new_etiquette(getE());
  operande *t = code3a_new_temporaire();
  
  operande *moinsUn = code3a_new_constante(-1);
  
  code3a_ajoute_instruction(assign,zero,NULL,t,NULL);
  
  code3a_ajoute_instruction(jump,e3,NULL,NULL,NULL);
  code3a_ajoute_etiquette(e2->u.oper_nom);
  code3a_ajoute_instruction(assign,moinsUn,NULL,t,NULL);
  
  code3a_ajoute_etiquette(e3->u.oper_nom);
  
  code3a_ajoute_instruction(jump_if_equal,t,zero,e1,NULL);
  parcours_instr(n->u.si_.alors);
  code3a_ajoute_etiquette(e1->u.oper_nom);
  /*
  if(n->u.si_.sinon){
    code3a_ajoute_etiquette(vrai->u.oper_nom);
    parcours_instr(n->u.si_.sinon);
  }
  code3a_ajoute_etiquette(suiv->u.oper_nom);
  * */
}

/*-------------------------------------------------------------------------*/

void parcours_instr_tantque(n_instr *n)
{
  operande *e0 = code3a_new_etiquette(getE());
  code3a_ajoute_etiquette(e0->u.oper_nom);
  operande *e1 = code3a_new_etiquette(getE());
  
  operande* t = code3a_new_temporaire();
  operande *zero = code3a_new_constante(0);
  operande *e2 = parcours_exp(n->u.si_.test);
  
  operande *e3 = code3a_new_etiquette(getE());
  
  operande *moinsUn = code3a_new_constante(-1);
  
  code3a_ajoute_instruction(assign,zero,NULL,t,NULL);
  
  code3a_ajoute_instruction(jump,e3,NULL,NULL,NULL);
  code3a_ajoute_etiquette(e2->u.oper_nom);
  code3a_ajoute_instruction(assign,moinsUn,NULL,t,NULL);
  
  code3a_ajoute_etiquette(e3->u.oper_nom);
   code3a_ajoute_instruction(jump_if_equal,t,zero,e1,NULL);
  
  parcours_instr(n->u.si_.alors);
  code3a_ajoute_instruction(jump,e0,NULL,NULL,NULL);
  code3a_ajoute_etiquette(e1->u.oper_nom);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_affect(n_instr *n)
{
	operande* op1 = parcours_var(n->u.affecte_.var);
	operande* op2 = parcours_exp(n->u.affecte_.exp);
	code3a_ajoute_instruction(assign,op2,NULL,op1,NULL);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_appel(n_instr *n)
{
  operande *tmp = parcours_appel(n->u.appel);
  code3a_ajoute_instruction(func_call,tmp,NULL,NULL,NULL);
}
/*-------------------------------------------------------------------------*/

operande* parcours_appel(n_appel *n)
{
  int ligne =rechercheExecutable(n->fonction);
  operande *un = code3a_new_constante(1);
  code3a_ajoute_instruction(alloc,un,NULL,NULL,NULL);
  if(ligne == -1){
		printf("Erreur semantique : fonction non declaree\n");
		return NULL;
	 }
   

  if(n->args==NULL)
    printf("\n");
  
   else if(tabsymboles.tab[ligne].complement!=nb_arguments_exp(n->args)){
     printf("Erreur semantique : fonction appelee avec le mauvais nombre d'args\n");
		return NULL;
   }
   int preportee = portee;
   portee = P_ARGUMENT;
  parcours_l_exp(n->args);
  portee = preportee;
  operande* tmp = code3a_new_etiquette(concatenate("f",n->fonction));
  operande *t = code3a_new_temporaire();
  //code3a_ajoute_instruction(func_call,tmp,NULL,NULL,NULL);
  code3a_ajoute_instruction(assign,func_call,tmp,t,NULL);
  
  return tmp;
}

/*-------------------------------------------------------------------------*/

void parcours_instr_retour(n_instr *n)
{
  operande* tmp = parcours_exp(n->u.retour_.expression);
  code3a_ajoute_instruction(func_val_ret,tmp,NULL,NULL,NULL);
  code3a_ajoute_instruction(func_end,NULL,NULL,NULL,NULL);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n)
{
  operande* tmp = parcours_exp(n->u.ecrire_.expression);
  code3a_ajoute_instruction(sys_write,tmp,NULL,NULL,NULL);
}

/*-------------------------------------------------------------------------*/

void parcours_l_exp(n_l_exp *n)
{
  if(n){
    parcours_exp(n->tete);
    parcours_l_exp(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

operande* parcours_exp(n_exp *n)
{
  if(n->type == varExp) return parcours_varExp(n);
  else if(n->type == opExp) return parcours_opExp(n);
  else if(n->type == intExp) return parcours_intExp(n);
  else if(n->type == appelExp) return parcours_appelExp(n);
  else if(n->type == lireExp) return parcours_lireExp(n);
  return NULL;
}

/*-------------------------------------------------------------------------*/

operande* parcours_varExp(n_exp *n)
{
  return parcours_var(n->u.var);
}

/*-------------------------------------------------------------------------*/
operande* parcours_opExp(n_exp *n)
{
	operande *op1 = parcours_exp(n->u.opExp_.op1);
	operande *op2 = parcours_exp(n->u.opExp_.op2);
	
	operande *tmp;
	
	if(n->u.opExp_.op == plus || n->u.opExp_.op == moins || n->u.opExp_.op == fois || n->u.opExp_.op == divise ){
		tmp = code3a_new_temporaire();
		switch(n->u.opExp_.op){
			case plus : code3a_ajoute_instruction(arith_add,op1,op2,tmp,NULL); break;
			case moins : code3a_ajoute_instruction(arith_sub,op1,op2,tmp,NULL); break;
			case fois : code3a_ajoute_instruction(arith_mult,op1,op2,tmp,NULL); break;
			case divise : code3a_ajoute_instruction(arith_div,op1,op2,tmp,NULL); break;
			default : break;
		}
	}
	else{
		tmp = code3a_new_etiquette(getE());
		switch(n->u.opExp_.op){
			case egal : code3a_ajoute_instruction(jump_if_equal,op1,op2,tmp,NULL); break;
			case inferieur : code3a_ajoute_instruction(jump_if_less,op1,op2,tmp,NULL); break;
			case ou : code3a_ajoute_instruction(jump_if_equal,op1,op2,tmp,NULL); break;
			case et : code3a_ajoute_instruction(jump_if_equal,op1,op2,tmp,NULL); break;
			case non : code3a_ajoute_instruction(jump_if_equal,op1,op2,tmp,NULL); break;
			default : break;
		}	
	}
	return tmp;
}

/*-------------------------------------------------------------------------*/

operande* parcours_intExp(n_exp *n)
{
  char texte[ 50 ]; // Max. 50 chiffres
  sprintf(texte, "%d", n->u.entier);
  return code3a_new_constante(n->u.entier);
}

/*-------------------------------------------------------------------------*/
operande* parcours_lireExp(n_exp *n)
{
	operande *tmp = code3a_new_temporaire();
  code3a_ajoute_instruction(sys_read,NULL,NULL,tmp,NULL);
	return tmp;
}

/*-------------------------------------------------------------------------*/

operande* parcours_appelExp(n_exp *n)
{
  return parcours_appel(n->u.appel);
}

/*-------------------------------------------------------------------------*/

void parcours_l_dec(n_l_dec *n)
{
  if( n ){
    
    parcours_dec(n->tete);
    parcours_l_dec(n->queue);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_dec(n_dec *n)
{
  if(n){
    if(n->type == foncDec) {
      parcours_foncDec(n);
    }
    else if(n->type == varDec) {
      parcours_varDec(n);
    }
    else if(n->type == tabDec) { 
      parcours_tabDec(n);
    }
  }
}

char* getE(){
	nbE++;
	return concatenate("e",intToString(nbE));
}

char* concatenate(char* s1,char* s2){
	int i;
	int ss1 = strlen(s1);
	int ss2 = strlen(s2);
	char* s3 = malloc(sizeof(char)*(ss1+ss2+1));
	for(i=0;i<ss1;i++)
		s3[i] = s1[i];
	for(i=0;i<ss2;i++)
		s3[i+ss1] = s2[i];
	return s3;
}

/*-------------------------------------------------------------------------*/

void parcours_foncDec(n_dec *n)
{
	int ligne = rechercheDeclarative(n->nom);
	operande *tmp = code3a_new_etiquette(n->nom);
  
  if(ligne != -1){
		printf("Erreur semantique : Fonction deja declaree a ligne %d\n",ligne);
		return;
	}
  if(n->u.foncDec_.param==NULL){
    //tabsymboles.adresseGlobaleCourante
    ajouteIdentificateur(n->nom,portee,3,0,0);
  }
  else
  {
	  //tabsymboles.adresseGlobaleCourante
    ajouteIdentificateur(n->nom,portee,3,0,nb_arguments_dec(n->u.foncDec_.param));
    }
  
  code3a_ajoute_etiquette(concatenate("f",tmp->u.oper_nom));
  
  code3a_ajoute_instruction(func_begin,NULL,NULL,NULL,NULL);
  
	entreeFonction();
  adresseLocaleCourante = 0;
  
  parcours_l_dec(n->u.foncDec_.param);
  
  portee = P_VARIABLE_LOCALE;
  adresseLocaleCourante = 0;
  
  parcours_l_dec(n->u.foncDec_.variables);
  
  parcours_instr(n->u.foncDec_.corps);
  
  sortieFonction(trace_tabsymb);
  
  code3a_ajoute_instruction(func_end,NULL,NULL,NULL,NULL);
  
  portee = P_VARIABLE_GLOBALE;
  
}

/*-------------------------------------------------------------------------*/

void parcours_varDec(n_dec *n)
{
	int ligne = rechercheDeclarative(n->nom);
  if(ligne != -1){
    if(portee==tabsymboles.tab[ligne].portee){
		printf("Erreur semantique : Variable deja declaree a ligne %d dans la même portee\n",ligne);
		return;
    }
    else if ((portee==P_VARIABLE_LOCALE || portee == P_ARGUMENT) && (tabsymboles.tab[ligne].portee==P_VARIABLE_GLOBALE)){
          printf("Warning : Variable locale ou argument a le même nom qu'une variable globale");
      }
    else if(portee == P_VARIABLE_LOCALE && tabsymboles.tab[ligne].portee == P_ARGUMENT ) {
      printf("Erreur semantique : Variable locale deja declaree a ligne %d comme nom d'argument\n",ligne);
		return;
      } 
    }

	ajouteIdentificateur(n->nom,portee,1,adresseLocaleCourante,1);
	
	if(portee != P_ARGUMENT)
	{
		operande *var = code3a_new_var(n->nom,portee, tabsymboles.tab[ligne].adresse);
		operande *cst = code3a_new_constante(1);
		code3a_ajoute_instruction(alloc,cst,var,NULL,NULL);
	}
	adresseLocaleCourante += 4;
	//tabsymboles.addresseGlobaleCourante += 4;
  
}

/*-------------------------------------------------------------------------*/

void parcours_tabDec(n_dec *n)
{
	int ligne = rechercheDeclarative(n->nom);
  if(ligne != -1){
    
		printf("Erreur semantique : Variable deja declaree a ligne %d\n",ligne);
		return;
	}
	ajouteIdentificateur(n->nom,portee,1,adresseLocaleCourante,n->u.tabDec_.taille);
	
	operande *vartab = code3a_new_var(n->nom,portee, tabsymboles.tab[ligne].adresse);
	operande *taille = code3a_new_constante(n->u.tabDec_.taille);
	code3a_ajoute_instruction(alloc,taille,vartab,NULL,NULL);
	
	adresseGlobaleCourante += 4;
	//tabsymboles.addresseGlobaleCourante += 4*n->u.tabDec_.taille;
}

/*-------------------------------------------------------------------------*/

operande* parcours_var(n_var *n)
{
  if(n->type == simple) {
    return parcours_var_simple(n);
  }
  else if(n->type == indicee) {
    parcours_var_indicee(n);
    return NULL;
  }
  return NULL;
}

/*-------------------------------------------------------------------------*/
operande* parcours_var_simple(n_var *n)
{
  int ligne=rechercheExecutable(n->nom);
  if( ligne == -1){
		printf("Erreur semantique : Variable non declaree\n");
		return NULL;
	 } 
   if(n->u.indicee_.indice!=NULL){
     printf("Erreur : utilisation d un entier avec indice\n");
     return NULL;
   }
   operande* var = code3a_new_var(n->nom,portee,tabsymboles.tab[ligne].adresse);
   if(portee == P_ARGUMENT)
	code3a_ajoute_instruction(func_param,var,NULL,NULL,NULL);
   return var;
}

/*-------------------------------------------------------------------------*/
void parcours_var_indicee(n_var *n)
{

  if(rechercheExecutable(n->nom) == -1){
		printf("Erreur semantique : Variable non declaree\n");
		return;
	 } 
  if(n->u.indicee_.indice==NULL){
     printf("Erreur : utilisation d un tableau sans indice\n");
     return;
   }
  parcours_exp( n->u.indicee_.indice );
}
/*-------------------------------------------------------------------------*/

int nb_arguments_dec(n_l_dec *n){
	if(n == NULL)
		return 0;
	else{
    return nb_arguments_dec(n->queue)+1;	
  }
}

int nb_arguments_exp(n_l_exp *n){
	if(n == NULL)
		return 0;
	else return nb_arguments_exp(n->queue)+1;	
}

