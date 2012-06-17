#include "person.h"

Person::Person(int id, int fromFloor, int toFloor) {
	this->id = id;
	this->arrivalFloor = fromFloor;
	this->destinationFloor = toFloor;
	this->arrivedAt = time(NULL);
}
Person::Person(const Person& copy) {
	this->id = copy.id;
	this->arrivalFloor     = copy.arrivalFloor;
	this->arrivedAt        = copy.arrivedAt;
	this->startedTravelAt  = copy.startedTravelAt;
	this->endedTravelAt    = copy.endedTravelAt;
	this->destinationFloor = copy.destinationFloor;
}

void Person::startTravel() {
	this->startedTravelAt = time(NULL);
}

void Person::endTravel() {
	this->endedTravelAt = time(NULL);
}
