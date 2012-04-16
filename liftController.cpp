#include "liftController.h"

LiftController::LiftController() {
  // cierra los pipes que no necesita
  // incializa los sig handlers
}

int LiftController::work() {
  while(simRunning()) {
    waitGenteEnElSistema();
    // proteger sigint, sigterm
    {
      determinarProximoPiso();
      viajarUnPiso();
      // subirOBajarTimer();
      // esperarFinTimer();
      bajarPersonas();
      subirPersonas();
    }
  }
  return 0;
}

