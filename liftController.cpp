#include "liftController.h"
#include "lift.h"

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

template <class T> const T& min ( const T& a, const T& b ) {
    return (a>b)?b:a;     // or: return comp(a,b)?b:a; for the comp version
}

LiftController::LiftController(SetPuertas puertas, unsigned int capacidad) :
  log("LiftController") , puertas(puertas) ,
  liftStates(1, LiftState(1)), liftMailboxes(2, LiftMailbox(1)) {

  numberOfFloors = puertas.getCantidadDePuertas();
	this->continuarSimulacion = true;
}

int LiftController::work() {

  while(simRunning()) {
		log.info( "Esperando que haya movimiento!" );
    mailbox.receiveMessage(this);
  }
  log.info( "Termino el LiftController" );
  log.debug( "Send exit to lift" );
  // Send exit to each mailbox
  std::vector<LiftMailbox>::iterator liftMailbox;
  for(liftMailbox  = liftMailboxes.begin();
      liftMailbox != liftMailboxes.end();
      liftMailbox++) {
    liftMailbox->endWork();
  }

  log.debug( "Termino el LiftController ok" );
  return 0;
}

LiftController::~LiftController() {}

void LiftController::newLiftArrival(LiftState liftState) {
  log.info( "Llegó un ascensor!" );

  unsigned int liftId = liftState.getLiftId();
  liftStates.insert( liftStates.begin()+liftId, liftState );

  if (liftState.hasPeopleToGetOff()) {
    liftMailboxes[liftId].getOff();
  }

  switch(liftState.getMovingDirection()) {
    case NOT_MOVING:
      if (peopleWaitingUp(liftState.getCurrentFloor())) {
        getOnUp(liftId);
        liftMailboxes[liftId].travel(UP);
      } else if (peopleWaitingDown(liftState.getCurrentFloor())) {
        getOnDown(liftId);
        liftMailboxes[liftId].travel(DOWN);
      }
      break;
    case UP:
      if (peopleWaitingUp(liftState.getCurrentFloor())) {
        getOnUp(liftId);
        liftMailboxes[liftId].travel(UP);
      } else {
        liftMailboxes[liftId].travel(NOT_MOVING);
      }
      break;
    case DOWN:
      if (peopleWaitingDown(liftState.getCurrentFloor())) {
        getOnDown(liftId);
        liftMailboxes[liftId].travel(DOWN);
      } else {
        liftMailboxes[liftId].travel(NOT_MOVING);
      }
      break;
  }
}

void LiftController::newPersonArrival(Person person) {
  log.info( "Hay gente!!" );

  peopleWaiting.push_back(person);

  // Any lift travelling there => return
  std::vector<LiftState>::iterator state;
  for(state =  liftStates.begin();
      state != liftStates.end();
      state++) {
    if (state->goingTo(person.getArrivalFloor()))
      return;
  }

	
  // Any lift not travelling? => put it to work
  for(int i = 0; i < liftStates.size(); i++) {

    if (!liftStates[i].isMoving()) {
      MovingDirection direction =
        liftStates[i].getCurrentFloor() > person.getArrivalFloor() ? DOWN :
        liftStates[i].getCurrentFloor() < person.getArrivalFloor() ? UP : NOT_MOVING;

      liftMailboxes[i].travel(direction);
      return;
    }
	}

}

bool LiftController::peopleWaitingUp(unsigned int currentFloor) {

  bool waitingUp = false;
  std::vector<Person>::iterator person;

  for(person =  peopleWaiting.begin();
      person != peopleWaiting.end();
      person++) {
    // Anyone in this floor wants to go up?
    waitingUp |= person->getArrivalFloor() == currentFloor && person->travelsUp();
    // Anyone in a floor above waiting?
    waitingUp |= person->getArrivalFloor() > currentFloor;
  }

  return waitingUp;
}

bool LiftController::peopleWaitingDown(unsigned int currentFloor) {
  bool waitingDown = false;
  std::vector<Person>::iterator person;

  for(person = peopleWaiting.begin();
      person != peopleWaiting.end();
      person++) {
    // Anyone in this floor wants to go down?
    waitingDown |= person->getArrivalFloor() == currentFloor && person->travelsDown();
    // Anyone in a floor below waiting?
    waitingDown |= person->getArrivalFloor() < currentFloor;
  }

  return waitingDown;
}

void LiftController::getOnUp(unsigned int liftId) {

  LiftState liftState = liftStates[liftId];
  if (liftState.isFull()) return;

  unsigned int currentFloor = liftState.getCurrentFloor();

  for(unsigned int i = 0; i < liftState.getAvailableSpace(); i ++) {
    bool found = false;
    for(unsigned int j = 0; j < peopleWaiting.size(); j++) {
      if (peopleWaiting[j].getArrivalFloor() == currentFloor
        && peopleWaiting[j].travelsUp()) {
        found = true;
        liftMailboxes[liftId].getOn(peopleWaiting[j]);
        peopleWaiting.erase(peopleWaiting.begin() + j);
        break;
      }
    }
    if(!found) break;
  }
}

void LiftController::getOnDown(unsigned int liftId) {

  LiftState liftState = liftStates[liftId];
  if (liftState.isFull()) return;

  unsigned int currentFloor = liftState.getCurrentFloor();

  for(unsigned int i = 0; i < liftState.getAvailableSpace(); i ++) {
    bool found = false;
    for(unsigned int j = 0; j < peopleWaiting.size(); j++) {
      if (peopleWaiting[j].getArrivalFloor() == currentFloor
        && peopleWaiting[j].travelsDown()) {
        found = true;
        liftMailboxes[liftId].getOn(peopleWaiting[j]);
        peopleWaiting.erase(peopleWaiting.begin() + j);
        break;
      }
    }
    if(!found) break;
  }
}

void LiftController::endPeopleGenerator() {
	this->continuarSimulacion = false;
}
