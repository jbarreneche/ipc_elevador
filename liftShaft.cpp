#include "liftShaft.h"
#include "lift.h"
#include "liftController.h"
// #include "pipe.h"

#include <sys/wait.h>

#include <iostream>

#define NUMBER_OF_LIFTS 3

LiftShaft::LiftShaft(unsigned int cantidadPisos, unsigned int tiempoEntrePisos, unsigned int capacidadAscensor) :
      log("LiftShaft") {
  this->capacidadAscensor = capacidadAscensor;
  this->tiempoEntrePisos = tiempoEntrePisos;
  this->cantidadPisos = cantidadPisos;
}

int LiftShaft::run() {
  pid_t pid; int status;

  log.debug("run liftShaft");

  switch (pid = fork()) {
    case -1: return -1;
    case 0: {
      log.info("poniendo en marcha los ascensores!");

			for( int i = 0; i<NUMBER_OF_LIFTS; i++ )
				if( startNewLift(i) == 0 )
					return 0;

			waitAllLifts();

      log.info("esperando que el controller termine");

      return status;
    }
    default: {
      log.debug("poniendo en marcha el Controller!");

      LiftController liftController(NUMBER_OF_LIFTS);
      status = liftController.work();
      Logger::closeGlobalDebug(); // XXX: YUCK!
      waitpid(pid, &status, 0);

      return status;

    }
  }
}


int LiftShaft::startNewLift( int liftId ) {
  pid_t pid; int status;

  switch (pid = fork()) {
	  case -1: return -1;
    case 0: {
      log.info("poniendo en marcha el ascensor!");

      Lift lift(liftId, tiempoEntrePisos, capacidadAscensor);
      lift.start();

      return 0;

    }
    default: {
			this->liftPids.push_back( pid );
      return pid;
		}
	}
}

void LiftShaft::waitAllLifts() {
	int status;
  std::vector<pid_t>::iterator pids;

  for(pids  = liftPids.begin();
      pids != liftPids.end();
      pids++) {
		waitpid((*pids), &status, 0);
	}

	liftPids.clear();
}
