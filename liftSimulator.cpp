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

int buildSemaphore( int size );

int main() {
  Logger log;

  // Iniciar semaforos
  int semId = buildSemaphore( 3 );
  pid_t pid;

  log.debug( "start LiftSimulation" );
  switch (pid = fork()) {
    case -1:

      perror("fork");
      return(-1);

    case 0:
      {
        LiftShaft l(semId); // recibe los semaforos ??
        return l.run();
      }

    default:
      {
        PeopleGenerator g(pid, semId); // recibe los semaforos ??  y TTotal de simulacion
        g.run(5);

      }
      wait(NULL);
  }

  return 0;
}

int buildSemaphore( int size ) {
  key_t key = ftok( "liftSim", 0);
  int semId = semget(key, size, IPC_CREAT | 0666 );

  int data[size] ;

  for(int i = 0; i < size; i++) data[i] = 0;

  semctl(semId, 0, SETALL, data); 

  return semId;
}
