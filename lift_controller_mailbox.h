#ifndef LIFT_CONTROLLER_MAILBOX_H_
#define LIFT_CONTROLLER_MAILBOX_H_

#include "person.h"
#include "liftController.h"
// #include "lift.h"

class LiftControllerMailbox {
public:
	LiftControllerMailbox();

	void newPersonArrival(Person);
	void liftArrival(liftControlledState);
	void receiveMessage(LiftController &);

private:
	int queueId;
};


#endif