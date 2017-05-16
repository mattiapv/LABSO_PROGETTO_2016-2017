#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>

/*void sigquit_handler (int sig) {
    assert(sig == SIGQUIT);
    printf("Padre uscito figlio esce");
    _exit(0);
} */

int main(){
   //signal(SIGQUIT, sigquit_handler);
   sleep(3600); // figlio resta a dormire per un'ora poi esce.
   _exit(0);
   return 0;
}
