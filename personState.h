#ifndef PERSON_STATE_H_
#define PERSON_STATE_H_

#include <time.h>

class PersonState {

public:

	PersonState(int id, unsigned int fromFloor, unsigned int numberOfFloors);
	PersonState(const PersonState&);

	unsigned int getArrivalFloor();
	unsigned int getDestinationFloor();
	int getId();

	friend class Person;

private:
	int id;
	unsigned int arrivalFloor, destinationFloor;
	time_t arrivedAt, startedTravelAt, endedTravelAt;
};

#endif
