#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

#include <iostream>
#include <vector>
#include <signal.h>

#include "logger.h"

enum MovingDirection { DOWN = -1, NOT_MOVING = 0, UP = 1 };

class LiftController {
  public:
    LiftController(int semId, int numberOfFloors);
    int work();
    ~LiftController();

    static void signalHandler( int signum) {
      LiftController::continuarSimulacion = 0;
      std::cout << "SIGINT to LiftController (pid=" << getpid() << ")" << std::endl;
    }

  private:

    Logger log;
    static volatile sig_atomic_t continuarSimulacion;

    int semId;
    int peopleTravelling, numberOfFloors, lugarDisponible;
    unsigned int nextFloor, currentFloor;
    MovingDirection movingDirection;
    std::vector<int> busyFloors;
    std::vector<int> requestedFloors;

    bool simRunning() {
      return ( LiftController::continuarSimulacion == 1 );
    }
    void waitGenteEnElSistema();
    void determinarProximoPiso();
    void viajarUnPiso();
    void bajarPersonas();
    void subirPersonas();
    bool isFull();
    void refreshBusyFloors() {}
    void updateMovingDirection();
    int findNearestBelow();
    int findNearestAbove();
    unsigned int randFloor();
};


#endif
