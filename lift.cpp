#include "lift.h"

#include <sstream>
#include <stdio.h>

#include <iostream>

Lift::Lift( unsigned int liftId, unsigned int delayEntrePisos, unsigned int capacity) :
	mailbox(liftId), state(liftId, capacity), log("Lift") {
	this->delayEntrePisos = delayEntrePisos;
  controller.newLiftArrival(state);
}

Lift::~Lift() {
  this->mailbox.close();
}

void Lift::start() {
  log.info( "start lift" );

  running = true;
  while( running ) {
    mailbox.receiveMessage(this);
  }

  log.info("exit lift ok");

}

void Lift::travel(MovingDirection direction) {
  state.movingDirection = direction;
  if (state.isMoving()) {
    int tiempoRestante = delayEntrePisos;
    while( tiempoRestante > 0 )
      tiempoRestante = sleep( tiempoRestante );
    state.currentFloor += state.getMovingDelta();
    logArrival();
  }
  state.peopleToGetOff  = countPeopleToGetOff();
  state.peopleRemaining = peopleTravelling.size() - state.peopleToGetOff;
  controller.newLiftArrival(state);
}

void Lift::logArrival() {
  std::stringstream ss;
  ss << state.getLiftId() << ": " << "llegó al piso " << state.currentFloor << " con " <<
  state.peopleToGetOff << " para bajar";
  log.info(ss.str().c_str());
}

void Lift::logEndTravel(Person person) {
  std::stringstream ss;
  ss << "Person(" << person.getId() << "): " << "se bajó del ascensor " << state.getLiftId() <<
  " en el piso " << person.getDestinationFloor();
  log.info(ss.str().c_str());
}

void Lift::logStartTravel(Person person) {
  std::stringstream ss;
  ss << "Person(" << person.getId() << "): " << "se tomó el ascensor " << state.getLiftId() <<
  " para viajar desde " << person.getArrivalFloor() << " hasta " << person.getDestinationFloor();
  log.info(ss.str().c_str());
}

void Lift::getOn(Person person) {
	this->peopleTravelling.push_back(person);
  person.startTravel();
  logStartTravel(person);
}

// Allow people wanting to get off on this floor
void Lift::getOff() {
  log.info("A punto de bajar personas");
	std::vector<Person>::iterator person;
	person = peopleTravelling.begin();

	while( person != peopleTravelling.end() ) {
		if( state.getCurrentFloor() == person->getDestinationFloor() ) {
      person->endTravel();
      logEndTravel(*person);
			person = peopleTravelling.erase(person);
		} else {
			person++;
		}
	}
}

void Lift::end() {
	log.info( "END_LIFT");
  running = false;
}

unsigned int Lift::countPeopleToGetOff() {

  unsigned int getOffCount = 0;
  std::vector<Person>::iterator person;
  person = peopleTravelling.begin();

  while( person != peopleTravelling.end() ) {
    if (person->getDestinationFloor() == state.currentFloor) {
      getOffCount++;
    }
    person++;
  }

  return getOffCount;
}