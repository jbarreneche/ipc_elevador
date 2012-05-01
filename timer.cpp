#include "timer.h"

int signalRegister2( int sigNum, void (*handler)(int) );

volatile sig_atomic_t Timer::killPid = 0;

Timer::Timer( int _speed, Pipe* inPipe, Pipe* outPipe ) :
	log("Lift") {

	this->speed = _speed;
	this->inPipe = inPipe;
	this->outPipe = outPipe;

	this->inPipe->setearModo(LECTURA);
	this->outPipe->setearModo(ESCRITURA);
}

void signalHandler( int signal ) {
  if (Timer::killPid) {
    kill(Timer::killPid, signal);
  }

}

void Timer::start(pid_t killPid) {
  Timer::killPid = killPid;
  std::cout << "I will kill " << Timer::killPid << std::endl;
  signalRegister2( SIGINT, &signalHandler );

  log.info( "start lift" );

  char buffer = LIFT_MOVE;

  while( buffer == LIFT_MOVE ) {
	  log.info("wait data");
	  inPipe->leer( &buffer, 1 );
	  log.info("Llego data");
	  switch( buffer ) {
	  case LIFT_MOVE:
		  log.debug("lift->move");
		  sleep(10);
		  log.debug("lift->respondo");
		  this->outPipe->escribir(LIFT_OK);
		  log.debug("lift->end response");
		  break;
	  case LIFT_EXIT:
		  log.debug("lift->exit");
		  this->outPipe->escribir(LIFT_OK);
		  sleep(5);
		  break;
	  }
  }
  log.debug("fin timer");


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
