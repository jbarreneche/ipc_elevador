#ifndef PERSON_H_
#define PERSON_H_

#include <time.h>
#include "personState.h"
#include "liftState.h"

#include "logger.h"

class Person {

public:
	Person(int id, unsigned int fromFloor, unsigned int numberOfFloors);
	Person(const Person&);
	Person(const PersonState&);
	void startTravel();
	void endTravel();
	unsigned int getArrivalFloor();
	unsigned int getDestinationFloor();
	bool travelsUp();
	bool travelsDown();

	int getId();

	PersonState getState();

	void startTravel( LiftState& state );
	void endTravel( LiftState& state );

private:
	PersonState state;
	Logger log;

};

#endif
