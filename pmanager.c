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

/*char *appendProcessNumber(alberoProcessi *albero){
   char *nome = malloc(15);
   sprintf(nome,"Processo%d",albero->numeroProcessi-1);
   return nome;
}*/

void killProcess(int pid){
   if (kill ( pid , SIGKILL ) < 0){
      int en = errno;
      printf("Error while trying to KILL process: %s\n", strerror(en));
   }
}

void catch_child(int sig_num) // funzione necessaria a ricevere segnali dal figlio
{
   int child_status;
   wait(&child_status);
   printf("Figlio terminato.\n");
}

// funzione per creare processo (pnew)
void creaProcesso(alberoProcessi *tree, char* line1){
   int  pid;
   pid = fork();
   if (pid < 0){
      printf("Errore fork !!\n");
      exit(1);
   }
   else if (pid == 0){ // processo figlio
      execl("child", line1, (char*) NULL); // sostituisco l'immagine del processo figlio con l'immagine di child
      printf("Figlio creato");
   }
   else {
      //char* nomeProcesso = appendProcessNumber(tree);
      addNodoProcesso(tree, pid, line1);
   }
}

//funzione per leggere da tastiera comando
char **lsh_read_line(){
   char *line = NULL;
   size_t bufsize = 0; // have getline allocate a buffer for us
   getline(&line, &bufsize, stdin);
   char ** res  = NULL;
   char *  p    = strtok (line, " ");
   int n_spaces = 0;
   /* split string and append tokens to 'res' */
   while (p!=NULL) {
      res = realloc (res, sizeof (char*) * ++n_spaces);
      if (res == NULL)
       exit (-1); /* memory allocation failed */
   res[n_spaces-1] = p;
   p = strtok (NULL, " ");
   }
   return res;
}

int main(){
   char** line = NULL;
   char* name ="Padre";
   int superPadrePid = getpid();
   alberoProcessi albero = creaAlbero();
   addNodoProcesso(&albero, superPadrePid, name); // Aggiungo il padre all'albero
   signal(SIGCHLD, catch_child);
   while (superPadrePid == getpid()) {
      printf("> ");
      line = lsh_read_line();
      if (line[0][strlen(line[0]) - 1] == '\n')
         line[0][strlen(line[0]) - 1] = '\0';
      if (strcmp(line[0], "quit") == 0){
         signal(SIGQUIT, SIG_IGN);
         kill(-superPadrePid, SIGQUIT);
         //sleep(1);
         break;
      }
      if (strcmp(line[0], "phelp") == 0){
         printf("phelp : stampa un elenco dei comandi disponibili \n");
         printf("plist : elenca i processi generati dalla shell custom \n");
         printf("pnew <nome> : crea un nuovo processo con nome <nome> \n");
         printf("pinfo <nome> : fornisce informazioni sul processo <nome> (almeno  pid  e  ppid ) \n");
         printf("pclose <nome> : chiede al processo <nome> di chiudersi \n");
         printf("quit : esce dalla shell custom \n");
      }
      else if (strcmp(line[0], "plist") == 0){
         stampaGerarchiaProcessi(&albero);
      }
      else if (strcmp(line[0], "pnew") == 0){
         while (line[1] == NULL){
            printf("Usare pnew + nome processo\n");
            printf("> ");
            line = lsh_read_line();
         }
         if (line[1][strlen(line[1]) - 1] == '\n')
            line[1][strlen(line[1]) - 1] = '\0';
         creaProcesso(&albero, line[1]);
      }
      else if (strcmp(line[0], "pclose") == 0){
         while (line[1] == NULL){
            printf("Usare pclose + nome processo\n");
            printf("> ");
            line = lsh_read_line();
         }
         if (line[1][strlen(line[1]) - 1] == '\n')
            line[1][strlen(line[1]) - 1] = '\0';
         int pid=-1;
         eliminaNodo(&albero, line[1], &pid);
         int val = pid;
         if (val>0){
            printf("Chiuso processo con pid: %d\n", pid);
            killProcess(val);
            sleep(1);
         }
         else
            printf("Processo %s non trovato\n", line[1]);
      }
      else if (strcmp(line[0], "pinfo") == 0){
         while (line[1] == NULL){
            printf("Usare pinfo + nome processo\n");
            printf("> ");
            line = lsh_read_line();
         }
         if (line[1][strlen(line[1]) - 1] == '\n')
            line[1][strlen(line[1]) - 1] = '\0';
         int pid=-1;
         infoNodo(&albero, line[1], &pid);
         int val = pid;
         if (val ==-1)
            printf("Processo %s inesistente \n", line[1]);
         else
            printf("Il pid del processo %s è %d e il ppid è %d\n", line[1], val, superPadrePid);
      }
      else printf("Usare phelp per la lista comandi\n");
   }
   fflush(stdout);
   free(line);
   return 0;
}
