#ifndef LIFT_CONTROLLER_MAILBOX_H_
#define LIFT_CONTROLLER_MAILBOX_H_

#include "person.h"
#include "liftState.h"

class LiftController;

class LiftControllerMailbox {
public:
	LiftControllerMailbox();

	void newPersonArrival(Person);
	void newLiftArrival(LiftState);
	void receiveMessage(LiftController*);
	void endPeopleGenerator();

	void close();
private:
	int queueId;
};

#include "liftController.h"

#endif
