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
void addNodoProcesso(alberoProcessi* tree, int pid, char* name){
   nodoProcesso *nuovoNodo = malloc(sizeof(nodoProcesso));
   nuovoNodo->pid = pid;
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

static void stampaProcesso(nodoProcesso *processo, int spazi){
   if (spazi==0){ // se la profondità è zero sono il padre quindi stampo senza spazi
      printf("%s, Pid: %d, Numero figli: %d\n", processo->processName, processo->pid, processo->numeroFigli);
   }
   else {
      printf("     | %s, Pid: %d, Numero figli: %d\n", processo->processName, processo->pid, processo->numeroFigli);
   }
}

static void stampaGerarchiaProcessiRic(nodoProcesso *nodo, int profondita){
   if (nodo == NULL)
      return;
   if (nodo->removed==false)
      stampaProcesso(nodo, profondita);
      stampaGerarchiaProcessiRic(nodo->fratello, profondita +1);
      stampaGerarchiaProcessiRic(nodo->primoFiglio, profondita +1);
}

void stampaGerarchiaProcessi(alberoProcessi *albero){
   stampaGerarchiaProcessiRic(albero->radice, 0);
}

static void eliminaNodoRic(nodoProcesso *nodo, char *nomeProcesso, int *pid){
   if (nodo == NULL)
      return;
   int ret = strcmp(nodo->processName, nomeProcesso);
   if (!ret){
      nodo->processName="Rimosso";
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

static void infoNodoRic(nodoProcesso *nodo, char *nomeProcesso, int *pid){
   if (nodo == NULL)
      return;
   int ret = strcmp(nodo->processName, nomeProcesso);
   if (!ret && nodo->removed==false){
      *pid=nodo->pid;
   }
   else {
      infoNodoRic(nodo->fratello, nomeProcesso, pid);
      infoNodoRic(nodo->primoFiglio, nomeProcesso, pid);
   }
}

void infoNodo(alberoProcessi *albero, char *nomeProcesso, int* pid) {
     infoNodoRic(albero->radice, nomeProcesso, pid);
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
