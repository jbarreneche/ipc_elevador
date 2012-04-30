#ifndef PEOPLE_GENERATOR
#define PEOPLE_GENERATOR

#include <signal.h>

#include "logger.h"
#include "setPuertas.h"

class PeopleGenerator {
  public:
    PeopleGenerator(SetPuertas puertas);

    void run(int simTime, pid_t pid);

  private:
    pid_t pid;
    Logger log;
    SetPuertas puertas;
    void spawnPerson();
};

#endif
