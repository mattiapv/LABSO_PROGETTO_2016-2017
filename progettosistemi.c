#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tree.h"
#include <signal.h>
#include <errno.h>


void killProcess(int pid){
   if (kill ( pid , SIGKILL ) < 0){
      int en = errno;
      printf("Error while trying to KILL process: %s\n", strerror(en));
   }
   //printf("Processo killato\n");
}

void catch_child(int sig_num) // funzione necessaria a ricevere segnali dal figlio
{
   int child_status;
   wait(&child_status);
   printf("Figlio terminato.\n");
}

// funzione per creare processo (pnew)
void creaProcesso(alberoProcessi *tree){
   int  pid;
   pid = fork();
   char* name ="Processo1"; // da sistemare con progressivo
   addNodoProcesso(tree, pid, name); // aggiungo il processo all'albero
   if (pid < 0){
      printf("Errore fork !!\n");
      exit(1);
   }
   else if (pid == 0){ // processo figlio
      execl("child", "Child Process", (char*) NULL); // sostituisco l'immagine del processo figlio con l'immagine di child
      printf("Figlio creato");
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
      if (strcmp(line[0], "quit\n") == 0){
         signal(SIGQUIT, SIG_IGN);
         kill(-superPadrePid, SIGQUIT);
         sleep(1);
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
         creaProcesso(&albero);
      }
      else if (strcmp(line[0], "pclose") == 0){
         printf("Case pclose \n");
         int val = atoi(line[1]);
         killProcess(val);
         sleep(1);
      }
      else if (strcmp(line[0], "pinfo") == 0)
         printf("Case pinfo\n");
      else printf("Usare phelp per la lista comandi\n");
   }
   fflush(stdout);
   free(line);
   return 0;
}
