#ifndef LIFT_SHAFT
#define LIFT_SHAFT

#include "logger.h"

class LiftShaft {
  public:
    LiftShaft(unsigned int cantidadPisos, unsigned int tiempoEntrePisos, unsigned int capacidadAscensor);
    int run();

  private:
    unsigned int capacidadAscensor;
    unsigned int tiempoEntrePisos;
    unsigned int cantidadPisos;
    Logger log;
};

#endif