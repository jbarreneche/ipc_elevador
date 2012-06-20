#include "lift.h"

#include <sstream>
#include <stdio.h>

#include <iostream>

Lift::Lift( unsigned int liftId, unsigned int delayEntrePisos) :
	mailbox(liftId), state(liftId), log("Lift") {
	this->delayEntrePisos = delayEntrePisos;
  controller.newLiftArrival(state);
}

void Lift::start(pid_t killPid) {
  log.info( "start lift" );

  running = true;
  while( running ) {
    mailbox.receiveMessage(this);
  }

	this->mailbox.close();
  log.debug("exit lift ok");

}

void Lift::travel(MovingDirection direction) {
  state.movingDirection = direction;
  if (direction == DOWN || direction == UP) {
    int tiempoRestante = delayEntrePisos;
    while( tiempoRestante > 0 )
      tiempoRestante = sleep( tiempoRestante );
    state.currentFloor += state.getMovingDelta();
    // Calc number of persons to get off
  }
  controller.newLiftArrival(state);
}

void Lift::getOn(Person person) {
	this->peopleTravelling.push_back(person);
	state.getOn();
}

// Allow people wanting to get off on this floor
void Lift::getOff() {
	std::vector<Person>::iterator persons;
	persons = peopleTravelling.begin();

	while( persons != peopleTravelling.end() ) {
	
		if( state.getCurrentFloor() == persons->getArrivalFloor() ) {
			state.getOff();
			persons = peopleTravelling.erase(persons);
		} else {
			persons++;
		}
	}

}

void Lift::end() {
	log.info( "END_LIFT");
  running = false;
}

