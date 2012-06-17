#include "lift_controller_mailbox.h"

#include <sys/ipc.h>
#include <sys/types.h>

#define PERSON_ARRIVAL_MESSAGE = 1L;
typedef struct personArrival {
	long mtype;
	Person person;
	personArrival(Person p) : mtype(1L), person(p)  {};
};

#define LIFT_ARRIVAL_MESSAGE = 2L;
typedef struct liftArrival {
	long mtype;
	liftControlledState state;
	liftArrival(liftControlledState s) : mtype(2L), state(s)  {};
};

struct longMessage {
	long mtype;
	char buff[200];
};

LiftControllerMailbox::LiftControllerMailbox() {
	// key_t mailboxKey = ftok("controller", 0);
	// queueId = msgget(mailboxKey, IPC_CREAT);
}

void LiftControllerMailbox::newPersonArrival(Person person) {
	personArrival message(person);
	// msgsnd(queueId, &message, sizeof(person), 0);
}

void LiftControllerMailbox::liftArrival(liftControlledState lift) {
	// liftArrival message = {
	// 	LIFT_ARRIVAL_MESSAGE, lift
	// };
	// msgsnd(queueId, &message, sizeof(lift), 0);
}

void LiftControllerMailbox::receiveMessage(LiftController &controller) {
	// longMessage undecodedMessage;
	// msgrcv(queueId, &undecodedMessage, sizeof(undecodedMessage.buff), 0, 0);
	// switch( undecodedMessage.mtype ) {
	// 	case LIFT_ARRIVAL_MESSAGE: {
	// 		liftArrival message = (liftArrival)undecodedMessage;
	// 		controller.newLiftArrival(message.state);
	// 		break;
	// 	}
	// 	case PERSON_ARRIVAL_MESSAGE: {
	// 		personArrival message = (personArrival)undecodedMessage;
	// 		controller.newPersonArrival(message.person);
	// 		break;
	// 	}
	// }
}
