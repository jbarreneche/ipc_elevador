#include "peopleGenerator.h"

#include <cstdlib>

#define MIN_SLEEP 1

PeopleGenerator::PeopleGenerator(int peopleTime, unsigned int numberOfFloors) :
  log("PeopleGenerator") {
  this->peopleTime = peopleTime;
	this->numberOfFloors = numberOfFloors;
	this->nextPersonId = 0;
}

void PeopleGenerator::run(int simTime) {
  time_t start = time(NULL);
  time_t end = start + simTime;

  while( time(NULL) < end ) {
    sleep( rand() % peopleTime + MIN_SLEEP );
    spawnPerson();
  }

  log.info( "Fin peopleGenerator. Mando mensaje a controller" );
	this->controllerMailBox.endPeopleGenerator();
}

void PeopleGenerator::spawnPerson() {
  log.info( "send person" );
	Person p( this->nextPersonId,
						rand() % this->numberOfFloors,
						this->numberOfFloors );

	this->controllerMailBox.newPersonArrival(p);
	this->nextPersonId += 1;
}

