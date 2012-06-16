#include "liftShaft.h"

LiftShaft::LiftShaft(SetPuertas puertas, unsigned int tiempoEntrePisos, unsigned int capacidadAscensor) :
      puertas(puertas), log("LiftShaft") {
  this->capacidadAscensor = capacidadAscensor;
  this->tiempoEntrePisos = tiempoEntrePisos;
}

int LiftShaft::run() {
  pid_t pid; int status;

  Pipe liftToController;
  Pipe controllerToLift;

  log.debug("run liftShaft");

  switch (pid = fork()) {
    case -1: return -1;
    case 0: {

      log.debug("poniendo en marcha el Controller!");

      LiftController liftController(puertas, capacidadAscensor, &liftToController, &controllerToLift);
      status = liftController.work();
      Logger::closeGlobalDebug(); // XXX: YUCK!

      return status;

    }
    default: {
      log.debug("poniendo en marcha el ascensor!");

      Lift lift(tiempoEntrePisos, &controllerToLift, &liftToController);
      lift.start(pid);

      log.debug("esperando que el controller termine");
      waitpid(pid, &status, 0);

      return status;
    }
  }
}
