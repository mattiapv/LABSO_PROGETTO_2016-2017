#include "tree.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

alberoProcessi creaAlbero(){
   alberoProcessi t;
   t.radice=NULL;
   t.numeroProcessi=1;
   return t;
}

// funzione che aggiunge all'albero dei processi il processo creando un nuovo nodo con relativo pid e nome del processo
void addNodoProcesso(alberoProcessi* tree, int pid, int ppid, char* name){
   nodoProcesso *nuovoNodo = malloc(sizeof(nodoProcesso));
   nuovoNodo->pid = pid;
   nuovoNodo->ppid = ppid;
   nuovoNodo->processName= name;
   nuovoNodo->primoFiglio   = NULL;
   nuovoNodo->fratello  = NULL;
   nuovoNodo->removed = false;
   if (tree->radice==NULL){ //Aggiunta del processo padre all'alberoProcessi
      nuovoNodo->numeroFigli=0;
      tree->radice=nuovoNodo;
      tree->numeroProcessi++;
   }
   else {
      nodoProcesso* nodoTemp = tree->radice;
      nodoTemp->numeroFigli++;
      tree->numeroProcessi++;
      if (nodoTemp->primoFiglio == NULL){ //Controllo se il padre ha già un figlio
         nodoTemp->primoFiglio = nuovoNodo;
      }
      else {
         nodoTemp = nodoTemp->primoFiglio;
            while (nodoTemp->fratello != NULL){ //Avanzo tra i fratelli e inserisco alla fine
            nodoTemp= nodoTemp->fratello;
            }
         nodoTemp->fratello=nuovoNodo;
      }
   }
}
// funzione stampaPtree
static void stampaPtree(nodoProcesso *processo, int spazi){
   if (spazi==0){
       // se la profondità è zero sono il padre quindi stampo senza spazi
      printf("\\_ %s, Pid: %d, Numero figli: %d\n", processo->processName, processo->pid, processo->numeroFigli);
   }
   else {
      printf("|");
      int i;
      for (i=0; i<=spazi; i++)
         printf("   ");
      printf("\\_ %s, Pid: %d, Numero figli: %d\n", processo->processName, processo->pid, processo->numeroFigli);
   }
}

// funzione stampaPlist
static void stampaPlist(nodoProcesso *processo){
      if (processo->removed==true)
         printf("Nome: %s, Pid: %d, Numero figli: %d, Stato: Chiuso\n", processo->processName, processo->pid, processo->numeroFigli);
      else
         printf("Nome: %s, Pid: %d, Numero figli: %d, Stato: Attivo\n", processo->processName, processo->pid, processo->numeroFigli);
}

//Funzione stampa generale
static void stampaGerarchiaProcessiRic(nodoProcesso *nodo, int profondita, int tipo){
   if (nodo == NULL)
      return;
   if (tipo){
      if (nodo->removed==false)
         stampaPtree(nodo, profondita);
      }
   else
      stampaPlist(nodo);
   stampaGerarchiaProcessiRic(nodo->primoFiglio, profondita +1, tipo);
   stampaGerarchiaProcessiRic(nodo->fratello, profondita, tipo);
}
//Funzione stampagenerale
void stampaGerarchiaProcessi(alberoProcessi *albero, int tipo){
   stampaGerarchiaProcessiRic(albero->radice, 0, tipo);
}

static void eliminaNodoRic(nodoProcesso *nodo, char *nomeProcesso, int *pid){
   if (nodo == NULL)
      return;
   int ret = strcmp(nodo->processName, nomeProcesso);
   if (!ret){
      sprintf(nodo->processName, "%s (Rimosso)",nodo->processName);
      nodo->removed=true;
      *pid=nodo->pid;
   }
   else {
      eliminaNodoRic(nodo->fratello, nomeProcesso, pid);
      eliminaNodoRic(nodo->primoFiglio, nomeProcesso, pid);
   }
}

void eliminaNodo(alberoProcessi *albero, char *nomeProcesso, int* pid) {
     eliminaNodoRic(albero->radice, nomeProcesso, pid);
}

static void infoNodoRic(nodoProcesso *nodo, char *nomeProcesso, int *pid, int* ppid){
   if (nodo == NULL)
      return;
   int ret = strcmp(nodo->processName, nomeProcesso);
   if (!ret && nodo->removed==false){
      *pid=nodo->pid;
      *ppid=nodo->ppid;
   }
   else {
      infoNodoRic(nodo->fratello, nomeProcesso, pid, ppid);
      infoNodoRic(nodo->primoFiglio, nomeProcesso, pid, ppid);
   }
}

void infoNodo(alberoProcessi *albero, char *nomeProcesso, int* pid, int*ppid) {
     infoNodoRic(albero->radice, nomeProcesso, pid, ppid);
}

static void controlloNomeRic(nodoProcesso *nodo, char* line, bool* nomeProcesso){
   if (nodo == NULL)
      return;
   int ret = strcmp(nodo->processName, line);
   if (!ret && nodo->removed==false){
      *nomeProcesso=true;
   }
   else {
      controlloNomeRic(nodo->fratello, line, nomeProcesso);
      controlloNomeRic(nodo->primoFiglio, line, nomeProcesso);
   }
}

void controlloNome(alberoProcessi *albero, char* line, bool *nomeProcesso) {
   controlloNomeRic(albero->radice, line, nomeProcesso);
}

void aggiornaNumeroProcessi(alberoProcessi *albero){
   albero->numeroProcessi--;
   albero->radice->numeroFigli--;
}
