#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

void handler (int signum) {
   if (signum == SIGUSR1)
    _exit(0);
}

int main(){
   signal(SIGUSR1,handler);
   pause();
   return EXIT_SUCCESS;
}
