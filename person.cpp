#include "person.h"
#include <stdlib.h>

unsigned int randFloor(unsigned int excludeFloor, unsigned int numberOfFloors) {
  unsigned int floor = rand() % (numberOfFloors - 1);
  if (floor >= excludeFloor) {
    return floor + 1;
  } else {
    return floor;
  }
}
Person::Person(int id, unsigned int fromFloor, unsigned int numberOfFloors) {
	this->id = id;
	this->arrivalFloor = fromFloor;
	this->destinationFloor = randFloor(fromFloor, numberOfFloors);
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
unsigned int Person::getArrivalFloor() {
	return arrivalFloor;
}

bool Person::travelsUp() {
	return arrivalFloor < destinationFloor;
}

bool Person::travelsDown() {
	return arrivalFloor > destinationFloor;
}
