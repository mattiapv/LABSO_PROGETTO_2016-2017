#ifndef TREE_H
#define TREE_H
#include <stdbool.h>

// struttura del nodo dell'albero
typedef struct nodoTree{
   int pid;
   int ppid;
   char* processName;
   struct nodoTree* primoFiglio;
   struct nodoTree* fratello;
   int numeroFigli;
   bool removed;
}nodoProcesso;

typedef struct tree{
   nodoProcesso* radice; // puntatore alla radice dell'albero
   int numeroProcessi; // numero processi totali
} alberoProcessi;


alberoProcessi creaAlbero();

void addNodoProcesso(alberoProcessi* tree, int pid, int ppid, char* processName);

void stampaGerarchiaProcessi(alberoProcessi* albero, int tipo);

void eliminaNodo(alberoProcessi *albero, char* nomeProcesso, int* pid);

void infoNodo(alberoProcessi *albero, char *nomeProcesso, int* pid, int* ppid);

void controlloNome(alberoProcessi *albero, char* line, bool* nomeProcesso);

void aggiornaNumeroProcessi(alberoProcessi *albero);

#endif
