#ifndef LIFT_SHAFT
#define LIFT_SHAFT

#include "liftController.h"
#include "timer.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "logger.h"
#include "pipe.h"

class LiftShaft {
  public:
    LiftShaft(SetPuertas puertas, unsigned int tiempoEntrePisos, unsigned int capacidadAscensor);
    int run();

  private:
    SetPuertas puertas;
    unsigned int capacidadAscensor;
    unsigned int tiempoEntrePisos;
    Logger log;
};

#endif

