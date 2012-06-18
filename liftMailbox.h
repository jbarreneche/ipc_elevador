#ifndef LIFT_MAILBOX_H_
#define LIFT_MAILBOX_H_

#include "person.h"
#include "liftState.h"

class Lift;

class LiftMailbox {
public:
	LiftMailbox(unsigned int liftId);
	void travel(MovingDirection);
	void getOn(Person);
	void getOff();
	void endWork();
	void receiveMessage(Lift*);

private:
	int queueId;
};

#include "lift.h"

#endif