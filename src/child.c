#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
bool flag;

void handler1 (int signum) {
   if (signum == SIGUSR1)
    _exit(0);
}

void handler2 (int signum) {
   if (signum == SIGUSR2)
    flag=false;
}

int main(){
   flag=true;
   int pid;
   signal(SIGUSR1,handler1);
   signal(SIGUSR2,handler2);
   do {
      pause();
      if (!flag){
         pid = fork();
            if (pid < 0){
               printf("Errore fork !!\n");
               exit(1);
            }
            else if (pid != 0)
               printf("Processo Clonato\nProcesso con pid %d generato\n", pid);
         flag=true;
      }
   } while (1);
return EXIT_SUCCESS;
}
