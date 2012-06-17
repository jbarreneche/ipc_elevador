#ifndef PERSON_H_
#define PERSON_H_

#include <time.h>

class Person {

public:
	Person(int id, int fromFloor, int toFloor);
	Person(const Person&);
	void startTravel();
	void endTravel();

private:
	int id;
	int arrivalFloor, destinationFloor;
	time_t arrivedAt, startedTravelAt, endedTravelAt;
};

#endif