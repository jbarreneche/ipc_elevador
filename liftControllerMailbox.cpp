#include "liftControllerMailbox.h"

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#define PERSON_ARRIVAL_MESSAGE 1L
typedef struct personArrival {
	long mtype;
	Person person;
	personArrival(Person p) : mtype(PERSON_ARRIVAL_MESSAGE), person(p)  {};
};

#define LIFT_ARRIVAL_MESSAGE 2L
typedef struct liftArrival {
	long mtype;
	LiftState state;
	liftArrival(LiftState s) : mtype(LIFT_ARRIVAL_MESSAGE), state(s)  {};
};

struct longMessage {
	long mtype;
	char buff[200];
};

LiftControllerMailbox::LiftControllerMailbox() {
	key_t mailboxKey = ftok("controller", 0);
	queueId = msgget(mailboxKey, IPC_CREAT);
}

void LiftControllerMailbox::newPersonArrival(Person person) {
	personArrival message(person);
	msgsnd(queueId, &message, sizeof(person), 0);
}

void LiftControllerMailbox::newLiftArrival(LiftState lift) {
	liftArrival message(lift);
	msgsnd(queueId, &message, sizeof(lift), 0);
}

void LiftControllerMailbox::receiveMessage(LiftController *controller) {
	longMessage undecodedMessage;
	msgrcv(queueId, &undecodedMessage, sizeof(undecodedMessage.buff), 0, 0);
	switch( undecodedMessage.mtype ) {
		case LIFT_ARRIVAL_MESSAGE: {
			liftArrival message((const liftArrival&)undecodedMessage);
			controller->newLiftArrival(message.state);
			break;
		}
		case PERSON_ARRIVAL_MESSAGE: {
			personArrival message((const personArrival&)undecodedMessage);
			controller->newPersonArrival(message.person);
			break;
		}
	}
}
