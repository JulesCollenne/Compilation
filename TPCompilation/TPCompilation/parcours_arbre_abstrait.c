#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntabs.h"
#include "util.h"
#include "tabsymboles.h"
#include "parcours_arbre_abstrait.h"
#include "code3a.h"

void parcours_n_prog(n_prog *n);
void parcours_l_instr(n_l_instr *n);
void parcours_instr(n_instr *n);
void parcours_instr_si(n_instr *n);
void parcours_instr_tantque(n_instr *n);
void parcours_instr_affect(n_instr *n);
void parcours_instr_appel(n_instr *n);
void parcours_instr_retour(n_instr *n);
void parcours_instr_ecrire(n_instr *n);
void parcours_l_exp(n_l_exp *n);
operande* parcours_exp(n_exp *n);
operande* parcours_varExp(n_exp *n);
operande* parcours_opExp(n_exp *n);
operande* parcours_intExp(n_exp *n);
operande* parcours_lireExp(n_exp *n);
operande* parcours_appelExp(n_exp *n);
void parcours_l_dec(n_l_dec *n);
void parcours_dec(n_dec *n);
void parcours_foncDec(n_dec *n);
void parcours_varDec(n_dec *n);
void parcours_tabDec(n_dec *n);
operande* parcours_var(n_var *n);
operande* parcours_var_simple(n_var *n);
void parcours_var_indicee(n_var *n);
operande* parcours_appel(n_appel *n);

// etiquette : creer dabord sans positionner avant d'utiliser

extern int portee;
extern tabsymboles_ tabsymboles;
extern n_prog *n;
extern code3a_ code3a;
int num_etiquette=0;
/*-------------------------------------------------------------------------*/

char * num_etiquette_to_char(){
  
  int *num;
  num = &num_etiquette;
  char *nom= malloc (sizeof (*nom) * 10); 
  
  if(num_etiquette<10){
    nom[0]='e';  
    char numc= num_etiquette +'0';
    nom[1]= numc;
    nom[2]='\0';}
  else{
      strcat(nom, "e");
      char *nom1= malloc (sizeof (*nom1) *3);
      snprintf(nom1, (sizeof(*nom)*3), "%d", num_etiquette); 
      strcat(nom,nom1);
  }
  
  
  ++*num;
  
  return nom;
}

void parcours_n_prog(n_prog *n)
{
  portee=P_VARIABLE_GLOBALE;
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

/*-------------------------------------------------------------------------*/

void parcours_instr_si(n_instr *n)
{  

  
  operande *faux = code3a_new_etiquette(num_etiquette_to_char());
  operande *zero = code3a_new_constante(0);
  operande *tmp1 = code3a_new_etiquette(num_etiquette_to_char());
  
  operande *tmp = parcours_exp(n->u.si_.test);
  code3a_ajoute_instruction(jump_if_equal,tmp,zero,faux,NULL);
  parcours_instr(n->u.si_.alors);
  code3a_ajoute_instruction(jump,tmp1,NULL,NULL,NULL);
  if(n->u.si_.sinon){
    code3a_ajoute_etiquette(faux->u.oper_nom);
    parcours_instr(n->u.si_.sinon);
  }
  code3a_ajoute_etiquette(tmp1->u.oper_nom);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_tantque(n_instr *n)
{

   
  operande *faux = code3a_new_constante(0);
  operande *tmp1 = code3a_new_etiquette(num_etiquette_to_char());
  operande *tmp2 = code3a_new_etiquette(num_etiquette_to_char());


  code3a_ajoute_etiquette(tmp1->u.oper_nom);
  operande *tmp  = parcours_exp(n->u.tantque_.test);
  code3a_ajoute_instruction(jump_if_equal,tmp,faux,tmp2,NULL);
  parcours_instr(n->u.tantque_.faire);
  code3a_ajoute_instruction(jump,tmp1,NULL,NULL,NULL);
  code3a_ajoute_etiquette(tmp2->u.oper_nom);
 
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

  parcours_l_exp(n->args);

  operande* tmp = code3a_new_etiquette(n->fonction);
  return tmp;
}

/*-------------------------------------------------------------------------*/

void parcours_instr_retour(n_instr *n)
{
  operande *tmp = parcours_exp(n->u.retour_.expression);
  code3a_ajoute_instruction(func_val_ret,tmp,NULL,NULL,NULL);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n)
{
  operande *tmp = parcours_exp(n->u.ecrire_.expression);
  code3a_ajoute_instruction(sys_write,tmp,NULL,NULL,NULL);
}

/*-------------------------------------------------------------------------*/

void parcours_l_exp(n_l_exp *n)
{

  if(n){
    operande *tmp = parcours_exp(n->tete);
    if(tmp!=NULL){
      code3a_ajoute_instruction(func_param,tmp,NULL,NULL,NULL);
    }
    
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
	
	operande *tmp = code3a_new_temporaire();

  operande *zero = code3a_new_constante(0);
  operande *moinsun = code3a_new_constante(-1);

  
  operande *affect_negatif = code3a_new_etiquette(num_etiquette_to_char());
  operande *test0 = code3a_new_etiquette(num_etiquette_to_char());
	
	switch(n->u.opExp_.op){
		case plus : code3a_ajoute_instruction(arith_add,op1,op2,tmp,NULL); break;
		case moins : code3a_ajoute_instruction(arith_sub,op1,op2,tmp,NULL); break;
		case fois : code3a_ajoute_instruction(arith_mult,op1,op2,tmp,NULL); break;
		case divise : code3a_ajoute_instruction(arith_div,op1,op2,tmp,NULL); break;

		case egal : code3a_ajoute_instruction(jump_if_equal,op1,op2,affect_negatif,NULL);
                code3a_ajoute_instruction(assign,zero,NULL,tmp,NULL);
                code3a_ajoute_instruction(jump,test0,NULL,NULL,NULL);
                code3a_ajoute_etiquette(affect_negatif->u.oper_nom);
                code3a_ajoute_instruction(assign,moinsun,NULL,tmp,NULL);
                code3a_ajoute_etiquette(test0->u.oper_nom); 
                break;

		case inferieur : 
                      code3a_ajoute_instruction(jump_if_less,op1,op2,affect_negatif,NULL); 
                      code3a_ajoute_instruction(assign,zero,NULL,tmp,NULL);
                      code3a_ajoute_instruction(jump,test0,NULL,NULL,NULL);
                      code3a_ajoute_etiquette(affect_negatif->u.oper_nom);
                      code3a_ajoute_instruction(assign,moinsun,NULL,tmp,NULL);
                      code3a_ajoute_etiquette(test0->u.oper_nom);

                      break;


		case ou : code3a_ajoute_instruction(jump_if_equal,op1,op2,tmp,NULL); break;
		case et : code3a_ajoute_instruction(jump_if_equal,op1,op2,tmp,NULL); break;
		case non : code3a_ajoute_instruction(jump_if_equal,op1,op2,tmp,NULL); break;
		default : break;
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

/*-------------------------------------------------------------------------*/

void parcours_foncDec(n_dec *n)
{
	operande *tmp = code3a_new_etiquette(n->nom);
  
	int ligne = rechercheDeclarative(n->nom);
  
  if(ligne != -1){
		printf("Erreur semantique : Fonction deja declaree a ligne %d\n",ligne);
		return;
	}
  if(n->u.foncDec_.param==NULL){
    
    ajouteIdentificateur(n->nom,portee,3,0,0);
  }
  else
  {
    
    ajouteIdentificateur(n->nom,portee,3,0,nb_arguments_dec(n->u.foncDec_.param));
    } 
  
  code3a_ajoute_etiquette(tmp->u.oper_nom);
  code3a_ajoute_instruction(func_begin,NULL,NULL,NULL,NULL);
  
  
	entreeFonction();
  
  portee = P_VARIABLE_LOCALE;
  
  parcours_l_dec(n->u.foncDec_.param);
  
  parcours_l_dec(n->u.foncDec_.variables);
  
  parcours_instr(n->u.foncDec_.corps);
  
  sortieFonction(1);

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

	ajouteIdentificateur(n->nom,portee,1,tabsymboles.addresseGlobaleCourante,1);
  
	
	
	operande *var = code3a_new_var(n->nom,portee, tabsymboles.tab[ligne].adresse);
	operande *cst = code3a_new_constante(1);
	code3a_ajoute_instruction(alloc,cst,var,NULL,NULL);
	
	tabsymboles.addresseGlobaleCourante += 4;
  
}

/*-------------------------------------------------------------------------*/

void parcours_tabDec(n_dec *n)
{
	int ligne = rechercheDeclarative(n->nom);
  if(ligne != -1){
    
		printf("Erreur semantique : Variable deja declaree a ligne %d\n",ligne);
		return;
	}
	ajouteIdentificateur(n->nom,portee,1,tabsymboles.addresseGlobaleCourante,n->u.tabDec_.taille);
	
	operande *vartab = code3a_new_var(n->nom,portee, tabsymboles.tab[ligne].adresse);
	operande *taille = code3a_new_constante(n->u.tabDec_.taille);
	code3a_ajoute_instruction(alloc,taille,vartab,NULL,NULL);
	
	tabsymboles.addresseGlobaleCourante += 4*n->u.tabDec_.taille;
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
   return code3a_new_var(n->nom,portee,tabsymboles.tab[ligne].adresse);
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
	if(n->queue == NULL)
		return 0;
	else{
    return nb_arguments_dec(n->queue)+1;	
  }
}

int nb_arguments_exp(n_l_exp *n){
	if(n->queue == NULL)
		return 0;
	else return nb_arguments_exp(n->queue)+1;	
}

