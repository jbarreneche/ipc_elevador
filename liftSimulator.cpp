#include "liftShaft.h"
#include "peopleGenerator.h"
#include <sys/sem.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "logger.h"

int main() {
  srand ( time(NULL) );
  Logger log;
  SetPuertas puertas(10);

  // Iniciar semaforos
  pid_t pid;

  log.debug( "start LiftSimulation" );
  switch (pid = fork()) {
    case -1:

      perror("fork");
      return(-1);

    case 0:
      {
        LiftShaft l(&puertas); // recibe los semaforos ??
        return l.run();
      }

    default:
      {
        PeopleGenerator g(&puertas); // recibe los semaforos ??  y TTotal de simulacion
        g.run(5, pid);

      }
      waitpid(pid, NULL, 0);
  }

  return 0;
}

