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
	  void endPeopleGenerator();

  private:

    Logger log;
    bool continuarSimulacion;

    LiftControllerMailbox mailbox;
    SetPuertas puertas;

    unsigned int numberOfFloors;
    std::vector<LiftState> liftStates;
    std::vector<LiftMailbox> liftMailboxes;
    std::vector<Person> peopleWaiting;

	  bool simRunning() { return ( continuarSimulacion ); }
    bool peopleWaitingUp(unsigned int currentFloor);
    bool peopleWaitingDown(unsigned int currentFloor);
    void getOnUp(unsigned int liftId);
    void getOnDown(unsigned int liftId);
};

#endif
