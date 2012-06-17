#include "peopleGenerator.h"

#include <signal.h>

#define DELTA_SLEEP 10
#define MIN_SLEEP 3

PeopleGenerator::PeopleGenerator(int peopleTime, SetPuertas puertas) :
  log("PeopleGenerator"), puertas(puertas) {
  this->peopleTime = peopleTime;
}

void PeopleGenerator::run(int simTime, pid_t pid) {
  this->pid = pid;
  time_t start = time(NULL);
  time_t end = start + simTime;

  while( time(NULL) < end ) {
    sleep( rand() % peopleTime + MIN_SLEEP );
    spawnPerson();
  }

  log.info( "Fin peopleGenerator. Mando SIGINT a controller" );
  kill(pid, SIGINT);
}

void PeopleGenerator::spawnPerson() {
  puertas.agregarPersona( rand() % puertas.getCantidadDePuertas() );
}

