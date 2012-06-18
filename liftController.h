#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

#include "setPuertas.h"
#include "person.h"
#include "logger.h"

#include <vector>

#include "liftMailbox.h"
#include "liftControllerMailbox.h"

class LiftController {
  public:
    LiftController(SetPuertas puertas, unsigned int capacidad);
    ~LiftController();

    int work();
    void newLiftArrival(LiftState);
    void newPersonArrival(Person);

    static void signalHandler( int signum) {
      LiftController::continuarSimulacion = 0;
    }

  private:

    Logger log;
    static volatile sig_atomic_t continuarSimulacion;

    LiftControllerMailbox mailbox;
    SetPuertas puertas;

    unsigned int numberOfFloors;
    std::vector<LiftState> liftStates;
    std::vector<LiftMailbox> liftMailboxes;
    std::vector<Person> peopleWaiting;

    bool simRunning() { return ( LiftController::continuarSimulacion == 1 ); }
    bool peopleWaitingUp(unsigned int currentFloor);
    bool peopleWaitingDown(unsigned int currentFloor);
    void getOnUp(unsigned int liftId);
    void getOnDown(unsigned int liftId);
};

#endif
