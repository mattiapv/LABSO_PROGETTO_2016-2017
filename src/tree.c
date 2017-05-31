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

// funzione che aggiunge all'albero dei processi il processo creando un nuovo nodo con relativo pid e nome del processo.
// In questa funzione i nodi sono aggiunti soltanto come primo figlio del padre oppure come fratelli del primo figlio del padre.
void addNodoProcesso(alberoProcessi* tree, int pid, int ppid, char* name){
   nodoProcesso *nuovoNodo = malloc(sizeof(nodoProcesso));
   nuovoNodo->pid = pid;
   nuovoNodo->ppid = ppid;
   nuovoNodo->processName= name;
   nuovoNodo->numeroFigli= 0;
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
      if (processo->removed==true)
         printf("\\_ %s, Pid: %d, Numero figli: %d, Stato: Chiuso\n", processo->processName, processo->pid, processo->numeroFigli);
      else
         printf("\\_ %s, Pid: %d, Numero figli: %d, Stato: Attivo\n", processo->processName, processo->pid, processo->numeroFigli);
   }
   else {
      printf("|");
      int i;
      for (i=0; i<=spazi; i++)
         printf("   ");
      if (processo->removed==true)
         printf("\\_ %s, Pid: %d, Numero figli: %d, Stato: Chiuso\n", processo->processName, processo->pid, processo->numeroFigli);
      else
         printf("\\_ %s, Pid: %d, Numero figli: %d, Stato: Attivo\n", processo->processName, processo->pid, processo->numeroFigli);
   }
}

// funzione stampaPlist
static void stampaPlist(nodoProcesso *processo){
      if (processo->removed==false)
         printf("Nome: %s, Pid: %d, Numero figli: %d\n", processo->processName, processo->pid, processo->numeroFigli);
}


static void stampaGerarchiaProcessiRic(nodoProcesso *nodo, int profondita, int tipo){
   if (nodo == NULL)
      return;
   if (tipo){
      if (nodo->removed==false){
         stampaPlist(nodo);

      }
   }
   else{
      stampaPtree(nodo, profondita);
   }
      // debug printf("Entro ricorsione primoFiglio di %s\n", nodo->processName);
      stampaGerarchiaProcessiRic(nodo->primoFiglio, profondita +1, tipo);
      // debug printf("Entro ricorsione fratello di %s\n", nodo->processName);
      stampaGerarchiaProcessiRic(nodo->fratello, profondita, tipo);
}

//Funzione stampa generale
void stampaGerarchiaProcessi(alberoProcessi *albero, int tipo){
   stampaGerarchiaProcessiRic(albero->radice, 0, tipo);
}

// funzione che elimina il nodo dall'albero dei processi
static void eliminaNodoRic(nodoProcesso *nodo, char *nomeProcesso, int *pid){
   if (nodo == NULL)
      return;
   int ret = strcmp(nodo->processName, nomeProcesso);
   if (!ret){
      sprintf(nodo->processName, "%s (Rimosso)",nodo->processName); // cambio il nome del nodo chiuso per poter riutilizzare il suo nome.
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

// funzione che restituisce pid e ppid del nodo
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

// funzione per controllare se il nome è già nell'albero dei processi
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

// funzione che aggiunge i cloni dei figli del processo padre.
static void addNodopspawnRic(nodoProcesso *nodo, char *nomeProcesso){
   if (nodo == NULL)
      return;
   int ret = strcmp(nodo->processName, nomeProcesso);
   // debug printf("Mi trovo nel nodo: %s\n", nodo->processName);
   if (!ret && nodo->removed==false){
      char *nome = malloc(20);
      sprintf(nome,"%s_%d",nomeProcesso, nodo->numeroFigli+1);
      // debug printf("Il nome del nodo sarà: %s\n", nome);
      nodoProcesso *nuovoNodo = malloc(sizeof(nodoProcesso));
      nuovoNodo->pid = 2;
      nuovoNodo->ppid = nodo->pid;
      nuovoNodo->processName = nome;
      nuovoNodo->primoFiglio   = NULL;
      nuovoNodo->fratello  = NULL;
      nuovoNodo->removed = false;
      nuovoNodo->numeroFigli=0;
      nodo->numeroFigli++;
      free(nome);
      if (nodo->primoFiglio == NULL){ //Controllo se ha già un figlio
            nodo->primoFiglio = nuovoNodo;
            // debug printf("Inserisco come primoFiglio\n");
         }
         else {
            nodo = nodo->primoFiglio;
               while (nodo->fratello != NULL){ //Avanzo tra i fratelli e inserisco alla fine
               nodo= nodo->fratello;
               }
            // debug printf("Inserisco come fratello\n");
            nodo->fratello=nuovoNodo;
         }
      }
   else {
      addNodopspawnRic(nodo->primoFiglio, nomeProcesso);
      addNodopspawnRic(nodo->fratello, nomeProcesso);

   }
}

void addNodopspawn(alberoProcessi *albero, char *nomeProcesso) {
     addNodopspawnRic(albero->radice, nomeProcesso);
}
