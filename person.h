#ifndef PERSON_H_
#define PERSON_H_

#include <time.h>

class Person {

public:
	Person(int id, unsigned int fromFloor, unsigned int numberOfFloors);
	Person(const Person&);
	void startTravel();
	void endTravel();
	unsigned int getArrivalFloor();
	unsigned int getDestinationFloor();
	bool travelsUp();
	bool travelsDown();

	int getId();

private:
	int id;
	unsigned int arrivalFloor, destinationFloor;
	time_t arrivedAt, startedTravelAt, endedTravelAt;
};

#endif
