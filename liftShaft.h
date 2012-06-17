#ifndef LIFT_SHAFT
#define LIFT_SHAFT

#include "setPuertas.h"
#include "logger.h"

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