#include "lift.h"

#include <signal.h>
#include <sstream>

int signalRegister2( int sigNum, void (*handler)(int) );

volatile sig_atomic_t Lift::killPid = 0;

Lift::Lift( unsigned int liftId, unsigned int delayEntrePisos) :
	mailbox(liftId), state(liftId), log("Lift") {
	this->delayEntrePisos = delayEntrePisos;
  controller.newLiftArrival(state);
}

void signalHandler( int signal ) {
  if (Lift::killPid) {
    kill(Lift::killPid, signal);
  }
}

void Lift::start(pid_t killPid) {
  Lift::killPid = killPid;

  std::stringstream ss;
  ss << "on SIGINT kill procces pid=" << Lift::killPid;
  log.debug( ss.str().c_str() );

  signalRegister2( SIGINT, &signalHandler );

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
  running = false;
}

int signalRegister2( int sigNum, void (*handler)(int) ) {
  struct sigaction sa;

  sa.sa_handler = handler;
  sa.sa_flags = SA_RESTART;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sigaction");
    return -1;
  }

  return 0;
}
