#ifndef LIFT_SHAFT
#define LIFT_SHAFT

#include "liftController.h"
#include "timer.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

class LiftShaft {
  public:
    LiftShaft(SetPuertas puertas, unsigned int tiempoEntrePisos, unsigned int capacidadAscensor) :
      puertas(puertas), log("LiftShaft") {
      this->capacidadAscensor = capacidadAscensor;
      // initPipes();
    }

    int run() {
      log.debug("run liftShaft");
      pid_t pid;
      switch (pid = fork()) {
        case -1:
          return -1;
        case 0: {
                  LiftController liftController(puertas, capacidadAscensor);
                  log.debug("run liftController");
                  return liftController.work();
                }
        default:	
          log.debug("run timer");

          Timer t(30); // recibe pipes y parametros de tiempo
          t.start(pid);
          log.debug("waiting for son to finish");
          waitpid(pid, NULL, 0);
          return 0;
      }

    }

  private:
    SetPuertas puertas;
    unsigned int capacidadAscensor;
    Logger log;
};

#endif

