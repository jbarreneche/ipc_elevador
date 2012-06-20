#ifndef PEOPLE_GENERATOR
#define PEOPLE_GENERATOR

#include "logger.h"
#include "setPuertas.h"

#include "liftControllerMailbox.h"

class PeopleGenerator {
  public:

    PeopleGenerator(int peopleTime, unsigned int numberOfFloors);

    void run(int simTime, pid_t pid);

  private:
    pid_t pid;
    Logger log;
    int peopleTime;
    void spawnPerson();

	  unsigned int numberOfFloors;
	  unsigned int nextPersonId;
	  LiftControllerMailbox controllerMailBox;
};

#endif
