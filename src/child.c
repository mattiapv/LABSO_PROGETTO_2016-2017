#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>

bool flag;
void handler (int signum) {
   if (signum == SIGUSR1)
    exit(1);
}

int main(){
   signal(SIGUSR1,handler);
   pause();
   return EXIT_SUCCESS;
}
