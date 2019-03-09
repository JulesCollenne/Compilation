#include <stdio.h>
#include "syntabs.h"
#include "util.h"
#include "tabsymboles.h"
#include "parcours_arbre_abstrait.h"

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
void parcours_exp(n_exp *n);
void parcours_varExp(n_exp *n);
void parcours_opExp(n_exp *n);
void parcours_intExp(n_exp *n);
void parcours_lireExp(n_exp *n);
void parcours_appelExp(n_exp *n);
void parcours_l_dec(n_l_dec *n);
void parcours_dec(n_dec *n);
void parcours_foncDec(n_dec *n);
void parcours_varDec(n_dec *n);
void parcours_tabDec(n_dec *n);
void parcours_var(n_var *n);
void parcours_var_simple(n_var *n);
void parcours_var_indicee(n_var *n);
void parcours_appel(n_appel *n);



extern int portee;
extern int adresseLocaleCourante;
extern int adresseArgumentCourant;
int adresseGlobaleCourante = 0;

/*-------------------------------------------------------------------------*/

void parcours_n_prog(n_prog *n)
{
	portee = P_VARIABLE_GLOBALE;
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

  parcours_exp(n->u.si_.test);
  parcours_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    parcours_instr(n->u.si_.sinon);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr_tantque(n_instr *n)
{

  parcours_exp(n->u.tantque_.test);
  parcours_instr(n->u.tantque_.faire);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_affect(n_instr *n)
{


  parcours_var(n->u.affecte_.var);
  parcours_exp(n->u.affecte_.exp);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_appel(n_instr *n)
{
  parcours_appel(n->u.appel);
}
/*-------------------------------------------------------------------------*/

void parcours_appel(n_appel *n)
{
  int ligne =rechercheExecutable(n->fonction);
  if(ligne == -1){
		printf("Erreur semantique : fonction non declaree\n");
		return;
	 }
   

  if(n->args==NULL)
    printf("\n");
  
   else if(tabsymboles.tab[ligne].complement!=nb_arguments_exp(n->args)){
     printf("Erreur semantique : fonction appelee avec le mauvais nombre d'args\n");
		return;
   }
  parcours_l_exp(n->args);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_retour(n_instr *n)
{
  parcours_exp(n->u.retour_.expression);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n)
{
  parcours_exp(n->u.ecrire_.expression);
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

void parcours_exp(n_exp *n)
{
  if(n->type == varExp) parcours_varExp(n);
  else if(n->type == opExp) parcours_opExp(n);
  else if(n->type == intExp) parcours_intExp(n);
  else if(n->type == appelExp) parcours_appelExp(n);
  else if(n->type == lireExp) parcours_lireExp(n);
}

/*-------------------------------------------------------------------------*/

void parcours_varExp(n_exp *n)
{
  parcours_var(n->u.var);
}

/*-------------------------------------------------------------------------*/
void parcours_opExp(n_exp *n)
{

}

/*-------------------------------------------------------------------------*/

void parcours_intExp(n_exp *n)
{
  char texte[ 50 ]; // Max. 50 chiffres
  sprintf(texte, "%d", n->u.entier);
}

/*-------------------------------------------------------------------------*/
void parcours_lireExp(n_exp *n)
{
  

}

/*-------------------------------------------------------------------------*/

void parcours_appelExp(n_exp *n)
{
  parcours_appel(n->u.appel);
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
  
	int ligne = rechercheDeclarative(n->nom);
  
  if(ligne != -1){
		printf("Erreur semantique : Fonction deja declaree a ligne %d\n",ligne);
		return;
	}
  if(n->u.foncDec_.param==NULL){
    
    ajouteIdentificateur(n->nom,portee,3,adresseGlobaleCourante,0);
  }
  else
  {
    
    ajouteIdentificateur(n->nom,portee,3,adresseGlobaleCourante,nb_arguments_dec(n->u.foncDec_.param));
    }
  
	entreeFonction();
  
  parcours_l_dec(n->u.foncDec_.param);
  
  portee = P_VARIABLE_LOCALE;
  
  parcours_l_dec(n->u.foncDec_.variables);
  
  parcours_instr(n->u.foncDec_.corps);
  
  sortieFonction(1);
  
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
	tabsymboles.addresseGlobaleCourante += 4*n->u.tabDec_.taille;
}

/*-------------------------------------------------------------------------*/

void parcours_var(n_var *n)
{
  if(n->type == simple) {
    parcours_var_simple(n);
  }
  else if(n->type == indicee) {
    parcours_var_indicee(n);
  }
}

/*-------------------------------------------------------------------------*/
void parcours_var_simple(n_var *n)
{
  if(rechercheExecutable(n->nom) == -1){
		printf("Erreur semantique : Variable non declaree\n");
		return;
	 } 
   if(n->u.indicee_.indice!=NULL){
     printf("Erreur : utilisation d un entier avec indice\n");
     return;
   }
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
	if(n->tete == NULL)
		return 0;
	else{
    return nb_arguments_dec(n->queue)+1;	
  }
}

int nb_arguments_exp(n_l_exp *n){
	if(n->tete == NULL)
		return 0;
	else return nb_arguments_exp(n->queue)+1;	
}

