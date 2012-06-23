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

    bool simRunning();
    bool peopleWaitingUp(unsigned int currentFloor);
    bool peopleWaitingDown(unsigned int currentFloor);
    bool anyLiftMoving();
    void moveLiftUp(unsigned int liftId);
    void moveLiftDown(unsigned int liftId);
    void getOnUp(unsigned int liftId);
    void getOnDown(unsigned int liftId);
    void logPersonArrival(Person);
    void makeNextLiftMovement(LiftState, MovingDirection preferredDirection);
};

#endif
