#ifndef _LIFT_H_
#define _LIFT_H_

#include "person.h"
#include "liftState.h"
#include "liftMailbox.h"
#include "liftControllerMailbox.h"
#include "logger.h"

#include <vector>

#define DISTANCIA_PISOS   3

class Lift {
  public:
	Lift(unsigned int liftId, unsigned int delayEntrePisos, unsigned int capacity);
	~Lift();

	void start();
	void travel(MovingDirection);
	void getOn(Person);
	void getOff();
	void end();

  private:
	unsigned int delayEntrePisos;
	bool running;
	LiftMailbox mailbox;
	LiftControllerMailbox controller;
	LiftState state;
 	std::vector<Person> peopleTravelling;
	Logger log;

	void logArrival();
	unsigned int countPeopleToGetOff();
};

#endif
