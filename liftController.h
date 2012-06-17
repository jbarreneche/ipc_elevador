#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

#include "setPuertas.h"
#include "pipe.h"
#include "logger.h"

#include <vector>

enum MovingDirection { DOWN = -1, NOT_MOVING = 0, UP = 1, STOPPED };

struct liftControlledState {
    unsigned int nextFloor, currentFloor;
    MovingDirection movingDirection;
};

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
    unsigned int numberOfFloors, peopleTravelling, lugarDisponible;
    unsigned int currentLift; // Should disappear
    std::vector<liftControlledState> liftStates;
    std::vector<unsigned int> busyFloors;
    std::vector<unsigned int> requestedFloors;

    bool simRunning() { return ( LiftController::continuarSimulacion == 1 ); }
    void waitGenteEnElSistema();
    void viajarUnPiso(liftControlledState &state);
    void bajarPersonas(liftControlledState &state);
    void subirPersonas(liftControlledState &state);
    bool isFull();
    void refreshBusyFloors();
    void updateMovingDirection(liftControlledState &state);
    int findNearestBelow(liftControlledState &state);
    int findNearestAbove(liftControlledState &state);
    void logLiftState(liftControlledState state);
    unsigned int randFloor(unsigned int excludeFloor);
    MovingDirection determinarDireccionDeMovimiento(liftControlledState &state);
};

#endif
