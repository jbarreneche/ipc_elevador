#include "liftMailbox.h"

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#define TRAVEL_MESSAGE 1L
typedef struct travelMSG {
	long mtype;
	MovingDirection direction;
	travelMSG(MovingDirection dir) : mtype(TRAVEL_MESSAGE), direction(dir)  {};
};

#define GET_ON_MESSAGE 2L
typedef struct getOnMSG {
	long mtype;
	Person person;
	getOnMSG(Person p) : mtype(GET_ON_MESSAGE), person(p)  {};
};

#define GET_OFF_MESSAGE 3L
typedef struct getOffMSG {
	long mtype;
	getOffMSG() : mtype(GET_OFF_MESSAGE)  {};
};

#define END_WORK_MESSAGE 4L
typedef struct endWorkMSG {
	long mtype;
	endWorkMSG() : mtype(END_WORK_MESSAGE)  {};
};

struct longMessage {
	long mtype;
	char buff[200];
};

LiftMailbox::LiftMailbox(unsigned int liftId) {
	key_t mailboxKey = ftok("controller", liftId);
	queueId = msgget(mailboxKey, IPC_CREAT);
}

void LiftMailbox::travel(MovingDirection direction) {
	travelMSG message(direction);
	msgsnd(queueId, &message, sizeof(direction), 0);
}

void LiftMailbox::getOn(Person person) {
	getOnMSG message(person);
	msgsnd(queueId, &message, sizeof(person), 0);
}

void LiftMailbox::getOff() {
	getOffMSG message;
	msgsnd(queueId, &message, 0, 0);
}

void LiftMailbox::endWork() {
	endWorkMSG message;
	msgsnd(queueId, &message, 0, 0);
}

void LiftMailbox::receiveMessage(Lift* lift) {
	longMessage undecodedMessage;
	msgrcv(queueId, &undecodedMessage, sizeof(undecodedMessage.buff), 0, 0);
	switch( undecodedMessage.mtype ) {
		case TRAVEL_MESSAGE: {
			travelMSG message((const travelMSG&)undecodedMessage);
			lift->travel(message.direction);
			break;
		}
		case GET_ON_MESSAGE: {
			getOnMSG message((const getOnMSG&)undecodedMessage);
			lift->getOn(message.person);
			break;
		}
		case GET_OFF_MESSAGE: {
			lift->getOff();
			break;
		}
	}

}
