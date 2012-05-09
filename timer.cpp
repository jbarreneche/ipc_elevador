#include "timer.h"

int signalRegister2( int sigNum, void (*handler)(int) );

volatile sig_atomic_t Lift::killPid = 0;

Lift::Lift( unsigned int _delayEntrePisos, Pipe* inPipe, Pipe* outPipe ) :
	log("Lift") {

	this->delayEntrePisos = _delayEntrePisos;
	this->inPipe = inPipe;
	this->outPipe = outPipe;

	this->inPipe->setearModo(LECTURA);
	this->outPipe->setearModo(ESCRITURA);
}

void signalHandler( int signal ) {
  if (Lift::killPid) {
    kill(Lift::killPid, signal);
  }

}

void Lift::start(pid_t killPid) {
  Lift::killPid = killPid;
  std::cout << "I will kill " << Lift::killPid << std::endl;
  signalRegister2( SIGINT, &signalHandler );

  log.info( "start lift" );

  char buffer = LIFT_MOVE;

  while( buffer == LIFT_MOVE ) {

	  log.debug("waiting pipe");
	  inPipe->leer( &buffer, 1 );


	  switch( buffer ) {
	  case LIFT_MOVE: {
		  log.info("move");

		  sleep( delayEntrePisos );
		  //sleep(2);
		  log.debug("move ok");
		  this->outPipe->escribir(LIFT_OK);
		  break;
	  }
	  case LIFT_EXIT:
	  default:
		  log.info("exit");
		  this->outPipe->escribir(LIFT_OK);
		  break;
	  }
  }
  log.info("exit lift ok");


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
