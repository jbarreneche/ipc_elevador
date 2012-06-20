#include "lift.h"

#include <sstream>
#include <stdio.h>

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
  // Add person to current state
}

void Lift::getOff() {
  // Allow people wanting to get off on this floor
}

void Lift::end() {
	log.info( "END_LIFT");
  running = false;
}

