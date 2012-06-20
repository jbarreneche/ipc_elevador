#include "liftController.h"

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

template <class T> const T& min ( const T& a, const T& b ) {
    return (a>b)?b:a;     // or: return comp(a,b)?b:a; for the comp version
}

LiftController::LiftController(unsigned int liftsCount) :
  log("LiftController") ,
  liftStates(liftsCount, LiftState()) {

  for (unsigned int i = 0; i < liftsCount; i++) {
    LiftMailbox lMailbox(i);
    liftMailboxes.push_back(lMailbox);
  }

	this->continuarSimulacion = true;
  stoppedLifts = 0;
}

int LiftController::work() {

  while(simRunning()) {
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

LiftController::~LiftController() {
  this->mailbox.close();
}


void LiftController::newLiftArrival(LiftState liftState) {

  unsigned int liftId = liftState.getLiftId();
  liftStates.at(liftId) = liftState;

  if (liftState.hasPeopleToGetOff()) {
    log.info( "Llegó un ascensor con gente!" );
    liftMailboxes[liftId].getOff();
  } else {
    log.info( "Llegó un ascensor!" );
  }

  switch(liftState.getMovingDirection()) {
    case NOT_MOVING:
      if (peopleWaitingUp(liftState.getCurrentFloor())) {
        getOnUp(liftId);
        liftMailboxes[liftId].travel(UP);
      } else if (peopleWaitingDown(liftState.getCurrentFloor())) {
        getOnDown(liftId);
        liftMailboxes[liftId].travel(DOWN);
      } else {
        stoppedLifts++;
        log.info("Ascensor seguirá quieto (no hay nadie arriba ni abajo)");
      }
      break;
    case UP:
      if (!liftState.isEmpty() || peopleWaitingUp(liftState.getCurrentFloor())) {
        getOnUp(liftId);
        liftMailboxes[liftId].travel(UP);
      } else {
        liftMailboxes[liftId].travel(NOT_MOVING);
      }
      break;
    case DOWN:
      if (!liftState.isEmpty() || peopleWaitingDown(liftState.getCurrentFloor())) {
        getOnDown(liftId);
        liftMailboxes[liftId].travel(DOWN);
      } else {
        liftMailboxes[liftId].travel(NOT_MOVING);
      }
      break;
  }
}

void LiftController::newPersonArrival(Person person) {

  peopleWaiting.push_back(person);
  logPersonArrival(person);

  // Any lift travelling there => return
  std::vector<LiftState>::iterator state;
  for(state =  liftStates.begin();
      state != liftStates.end();
      state++) {
    if (state->goingTo(person.getArrivalFloor())) {
      log.info( "Había un ascensor en camino" );
      return;
    }
  }
  log.info( "Ningún ascensor estaba en camino" );

  // Any lift not travelling? => put it to work
  if (stoppedLifts > 0) {
    log.info( "Se encontró ascensor libre!" );
    stoppedLifts--;
    unsigned int i = stoppedLifts;
    MovingDirection direction =
      liftStates[i].getCurrentFloor() > person.getArrivalFloor() ? DOWN :
      liftStates[i].getCurrentFloor() < person.getArrivalFloor() ? UP : NOT_MOVING;

    liftMailboxes[i].travel(direction);
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

  if (waitingUp) {
    log.info("Se encontró gente esperando arriba del piso actual");
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

  if (waitingDown) {
    log.info("Se encontró gente esperando abajo del piso actual");
  }

  return waitingDown;
}

void LiftController::getOnUp(unsigned int liftId) {

  LiftState liftState = liftStates[liftId];
  if (liftState.isFull()) {
    log.info("El ascensor estaba lleno");
    return;
  }

  unsigned int currentFloor = liftState.getCurrentFloor();
  unsigned int getOnCount = 0;

  std::vector<Person>::iterator person;
  person = peopleWaiting.begin();

  while( getOnCount < liftState.getAvailableSpace() &&
    person != peopleWaiting.end()) {

    if (person->getArrivalFloor() == currentFloor && person->travelsUp()) {
      log.info("Subiendo persona (sube)");
      getOnCount++;
      liftMailboxes[liftId].getOn(*person);
      person = peopleWaiting.erase(person);
    } else {
      person++;
    }
  }
}

void LiftController::getOnDown(unsigned int liftId) {

  LiftState liftState = liftStates[liftId];
  if (liftState.isFull()) {
    log.info("El ascensor estaba lleno");
    return;
  }

  unsigned int currentFloor = liftState.getCurrentFloor();
  unsigned int getOnCount = 0;

  std::vector<Person>::iterator person;
  person = peopleWaiting.begin();

  while( getOnCount < liftState.getAvailableSpace() &&
    person != peopleWaiting.end()) {

    if (person->getArrivalFloor() == currentFloor && person->travelsDown()) {
      log.info("Subiendo persona (baja)");
      getOnCount++;
      liftMailboxes[liftId].getOn(*person);
      person = peopleWaiting.erase(person);
    } else {
      person++;
    }
  }
}

void LiftController::endPeopleGenerator() {
	this->continuarSimulacion = false;
}

void LiftController::logPersonArrival(Person person) {
  std::stringstream ss;
  ss << "Person(" << person.getId() << "): " << "llegó al piso: " << person.getArrivalFloor() << " para viajar hasta " << person.getDestinationFloor();
  log.info(ss.str().c_str());
}

bool LiftController::simRunning() {
  return continuarSimulacion || peopleWaiting.size() > 0 || stoppedLifts != liftMailboxes.size();
}

// bool LiftController::anyLiftNotEmpty() {
//   bool anyNotEmpty = false;
//   std::vector<LiftState>::iterator liftState;

//   for(liftState  = liftStates.begin();
//       liftState != liftStates.end();
//       liftState++) {
//     anyNotEmpty |= !liftState->isEmpty();
//   }

//   if (anyNotEmpty) {
//     log.info("Hay algún");
//   }

//   return allEmpty;
// }