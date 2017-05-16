#include "tree.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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
   if(nodo!= NULL){
      stampaProcesso(nodo, profondita);
      stampaGerarchiaProcessiRic(nodo->fratello, profondita +1);
      stampaGerarchiaProcessiRic(nodo->primoFiglio, profondita +1);

   }
}

void stampaGerarchiaProcessi(alberoProcessi *albero){
   stampaGerarchiaProcessiRic(albero->radice, 0);
}
