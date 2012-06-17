#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

#include "setPuertas.h"
#include "pipe.h"
#include "logger.h"

#include <vector>

enum MovingDirection { DOWN = -1, NOT_MOVING = 0, UP = 1, STOPPED };

class LiftController {
  public:
    LiftController(SetPuertas puertas, unsigned int capacidad, Pipe* inPipe, Pipe* outPipe);
    ~LiftController();

    int work();

    static void signalHandler( int signum) {
      LiftController::continuarSimulacion = 0;
    }

  private:

    Logger log;
    static volatile sig_atomic_t continuarSimulacion;

    Pipe* inPipe;
    Pipe* outPipe;

    SetPuertas puertas;
    unsigned int peopleTravelling, numberOfFloors, lugarDisponible;
    unsigned int nextFloor, currentFloor;
    MovingDirection movingDirection;
    std::vector<unsigned int> busyFloors;
    std::vector<unsigned int> requestedFloors;

    bool simRunning() { return ( LiftController::continuarSimulacion == 1 ); }
    void waitGenteEnElSistema();
    void viajarUnPiso();
    void bajarPersonas();
    void subirPersonas();
    bool isFull();
    void refreshBusyFloors();
    void updateMovingDirection();
    int findNearestBelow();
    int findNearestAbove();
    unsigned int randFloor();
    MovingDirection determinarDireccionDeMovimiento();
};

#endif
