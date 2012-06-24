#include "liftControllerMailbox.h"

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define PERSON_ARRIVAL_MESSAGE 1L
typedef struct personArrival {
	long mtype;
	PersonState personState;
	personArrival(Person p) : mtype(PERSON_ARRIVAL_MESSAGE), personState(p.getState())  {};
};

#define LIFT_ARRIVAL_MESSAGE 2L
typedef struct liftArrival {
	long mtype;
	LiftState state;
	liftArrival(LiftState s) : mtype(LIFT_ARRIVAL_MESSAGE), state(s)  {};
};

#define END_GENERATOR_MESSAGE 3L
typedef struct endPeopleGeneratorMsg {
	long mtype;
	int nextId;
	endPeopleGeneratorMsg( int nextId ) : mtype( END_GENERATOR_MESSAGE ), nextId(nextId) {};
};

struct longMessage {
	long mtype;
	char buff[200];
};

LiftControllerMailbox::LiftControllerMailbox() {
	key_t mailboxKey = ftok("./liftSim", 0);

	queueId = msgget(mailboxKey, 0666 | IPC_CREAT);

	if( queueId == -1 ) {
		perror("queueID:");
		exit(1);
	}
}

void LiftControllerMailbox::newPersonArrival(Person person) {
	personArrival message(person);
	msgsnd(queueId, &message, sizeof(message.personState), 0);
}

void LiftControllerMailbox::newLiftArrival(LiftState lift) {
	liftArrival message(lift);
	msgsnd(queueId, &message, sizeof(lift), 0);
}

void LiftControllerMailbox::endPeopleGenerator() {
	endPeopleGeneratorMsg message( 0 );
	msgsnd(queueId, &message, sizeof(int), 0);
}

void LiftControllerMailbox::receiveMessage(LiftController *controller) {
	longMessage undecodedMessage;

	ssize_t received = msgrcv(queueId, &undecodedMessage, 
														sizeof(undecodedMessage.buff), 0, 0);

	if( received == -1 ) {
		std::cout << "size:" << queueId;
		perror("msgrcv");
		exit(1);
	}

	switch( undecodedMessage.mtype ) {
		case LIFT_ARRIVAL_MESSAGE: {
			liftArrival message((const liftArrival&)undecodedMessage);
			controller->newLiftArrival(message.state);
			break;
		}
		case PERSON_ARRIVAL_MESSAGE: {
			personArrival message((const personArrival&)undecodedMessage);
			controller->newPersonArrival(Person(message.personState));
			break;
		}
	  case END_GENERATOR_MESSAGE: {
			endPeopleGeneratorMsg message((const endPeopleGeneratorMsg&)undecodedMessage);
			controller->endPeopleGenerator();
		}
	}
}

void LiftControllerMailbox::close() {
	msgctl( this->queueId, IPC_RMID, NULL );
}
