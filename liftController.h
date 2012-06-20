#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

#include "person.h"
#include "logger.h"
#include "liftMailbox.h"
#include "liftControllerMailbox.h"

#include <vector>

class LiftController {
  public:
    LiftController(unsigned int liftsCount);
    ~LiftController();

    int work();
    void newLiftArrival(LiftState);
    void newPersonArrival(Person);
    void endPeopleGenerator();

  private:

    Logger log;
    bool continuarSimulacion;

    LiftControllerMailbox mailbox;

    std::vector<LiftState> liftStates;
    std::vector<Person> peopleWaiting;
    std::vector<LiftMailbox> liftMailboxes;

    bool simRunning() { return ( continuarSimulacion ); }
    bool peopleWaitingUp(unsigned int currentFloor);
    bool peopleWaitingDown(unsigned int currentFloor);
    void getOnUp(unsigned int liftId);
    void getOnDown(unsigned int liftId);
};

#endif
