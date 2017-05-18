#ifndef TREE_H
#define TREE_H
#include <stdbool.h>

// struttura del nodo dell'albero
typedef struct nodoTree{
   int pid;
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

void addNodoProcesso(alberoProcessi* tree, int pid, char* processName);

void stampaGerarchiaProcessi(alberoProcessi* albero);

void eliminaNodo(alberoProcessi *albero, char* nomeProcesso, int* pid);

void infoNodo(alberoProcessi *albero, char *nomeProcesso, int* pid);

#endif
