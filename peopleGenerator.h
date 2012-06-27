#ifndef PEOPLE_GENERATOR
#define PEOPLE_GENERATOR

#include "logger.h"
#include "liftControllerMailbox.h"

class PeopleGenerator {
  public:

    PeopleGenerator(int peopleTime, unsigned int numberOfFloors);

    void run(int simTime);

  private:
    Logger log;
    int peopleTime;
    unsigned int numberOfFloors;
    unsigned int nextPersonId;
    LiftControllerMailbox controllerMailBox;

    void spawnPerson();

};

#endif
