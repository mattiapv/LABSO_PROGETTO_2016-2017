#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tree.h"
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

// funzione necessaria a ricevere segnali dai figli
void catch_child(int sig_num)
{
   int child_status, childPid;
   childPid = wait(&child_status);
   if (SIGCHLD == sig_num)
   printf("Figlio pid %d terminato\n", childPid);
}

// funzione per creare un nuovo processo e aggiungerlo all'albero dei processi
void creaProcesso(alberoProcessi *tree, char* nomeProcesso){
   int  pid;
   bool nome=false;
   controlloNome(tree, nomeProcesso, &nome); // funzione per controllare se c'è un processo con lo stesso nome
   if (!nome){
      pid = fork();
   if (pid < 0){
      printf("Errore fork !!\n");
      exit(1);
   }
   else if (pid == 0){ // processo figlio
      execl("child", nomeProcesso, (char*) NULL); // sostituisco l'immagine del processo figlio con l'immagine di child
   }
   else { // processo padre
      printf("Processo %s con pid %d generato\n", nomeProcesso, pid);
      addNodoProcesso(tree, pid, nomeProcesso); // funzione per aggiungere il processo creato all'albero dei processi
   }
}
   else
      printf("Nome processo già esistente, utilizzare un altro nome\n");
}

//funzione per leggere da tastiera i comandi
char **leggiInputTastiera(){
   char *line = NULL;
   size_t bufsize = 0; //
   getline(&line, &bufsize, stdin);
   char ** res  = NULL;
   char *  p    = strtok (line, " ");
   int numeroSpazi = 0;
   // divide i 2 input da tastiera.
   while (p!=NULL && numeroSpazi<2) {
      res = realloc (res, sizeof (char*) * ++numeroSpazi);
      if (res == NULL)
       exit (-1); /* memory allocation failed */
   res[numeroSpazi-1] = p;
   p = strtok (NULL, " ");
   }
   return res;
}

char **leggiInputTxt(FILE* fp){
   char *line = NULL;
   size_t bufsize = 0; //
   getline(&line, &bufsize, fp);
   char ** res  = NULL;
   char *  p    = strtok (line, " ");
   int numeroSpazi = 0;
   // divide i 2 input da tastiera.
   while (p!=NULL && numeroSpazi<2) {
      res = realloc (res, sizeof (char*) * ++numeroSpazi);
      if (res == NULL)
       exit (-1); /* memory allocation failed */
   res[numeroSpazi-1] = p;
   p = strtok (NULL, " ");
   }
   return res;
}

int main(int argc, char ** argv){
   char** line = NULL;
   FILE *fp;
   char* name ="Padre";
   if (argv[1] !=NULL){
      if ((fp =fopen(argv[1],"r"))==NULL)
        printf("Error open, file\n");
      }
   int superPadrePid = getpid();
   alberoProcessi albero = creaAlbero(); // creo l'albero dei processi
   addNodoProcesso(&albero, superPadrePid, name); // Aggiungo il padre all'albero
   signal(SIGCHLD, catch_child); // creo l'handler per gestire i segnali dai figli
   signal(SIGINT, SIG_IGN); // ctrl-c ignorato
   while (superPadrePid == getpid()) {
      if (argv[1] == NULL){
         printf("> ");
         line = leggiInputTastiera();
      }
      else{
         line = leggiInputTxt(fp);
      }
      if (line[0][strlen(line[0]) - 1] == '\n')
         line[0][strlen(line[0]) - 1] = '\0';
      if (strcmp(line[0], "quit") == 0){
         signal(SIGQUIT, SIG_IGN);
         kill(0, SIGQUIT); // chiudo tutti i figli all'uscita del processo padre
         break;
      }
      if (strcmp(line[0], "phelp") == 0){
         printf("phelp : stampa un elenco dei comandi disponibili \n");
         printf("plist : elenca i processi generati dalla shell custom \n");
         printf("pnew <nome> : crea un nuovo processo con nome <nome> \n");
         printf("pinfo <nome> : fornisce informazioni sul processo <nome> (almeno  pid  e  ppid ) \n");
         printf("pclose <nome> : chiede al processo <nome> di chiudersi \n");
         printf("pspawn <nome> : chiede al processo <nome> di clonarsi creando <nome_i> con i progressivo\n");
         printf("prmall <nome> : chiede al processo <nome> di chiudersi chiudendo anche eventuali cloni\n");
         printf("ptree : mostra la gerarchia completa dei processi generati attivi\n");
         printf("quit : esce dalla shell custom \n");
      }
      else if (strcmp(line[0], "plist") == 0){
         stampaGerarchiaProcessi(&albero);
      }
      else if (strcmp(line[0], "pnew") == 0){
         if (line[1] == NULL){
            printf("Usare pnew + nome processo\n");
         }
         else {
            if (line[1][strlen(line[1]) - 1] == '\n')
               line[1][strlen(line[1]) - 1] = '\0';
            creaProcesso(&albero, line[1]);
         }
      }
      else if (strcmp(line[0], "pclose") == 0){
         if (line[1] == NULL){
            printf("Usare pclose + nome processo\n");
         }
         else {
            if (line[1][strlen(line[1]) - 1] == '\n')
               line[1][strlen(line[1]) - 1] = '\0';
            int pid=-1;
            eliminaNodo(&albero, line[1], &pid);
            int val = pid;
            if (val>0){
               if ( kill(val, SIGUSR1)<0 ) {
                  int en = errno;
                  printf("Errore nella chiusura del processo: %s\n", strerror(en));
               }else
                  aggiornaNumeroProcessi(&albero);
                  if (argv[1] == NULL)
                     sleep(1);
            }
            else
               printf("Processo %s non trovato\n", line[1]);
         }
      }
      else if (strcmp(line[0], "pinfo") == 0){
         if (line[1] == NULL){
            printf("Usare pinfo + nome processo\n");
         }
         else if (line[1][strlen(line[1]) - 1] == '\n'){
            line[1][strlen(line[1]) - 1] = '\0';
            int pid=-1;
            infoNodo(&albero, line[1], &pid);
            int val = pid;
            if (val ==-1)
               printf("Processo %s inesistente \n", line[1]);
            else
               printf("Il pid del processo %s è %d e il ppid è %d\n", line[1], val, superPadrePid);
            }
      }
      else printf("Usare phelp per la lista comandi\n");
   }
   fflush(stdout);
   free(line);
   return 0;
}
