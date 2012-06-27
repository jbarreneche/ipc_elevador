#include "person.h"
#include <stdlib.h>

#include <sstream>

unsigned int randFloor(unsigned int excludeFloor, unsigned int numberOfFloors) {
  unsigned int floor = rand() % (numberOfFloors - 1);
  if (floor >= excludeFloor) {
    return floor + 1;
  } else {
    return floor;
  }
}

Person::Person(int id, unsigned int fromFloor, unsigned int numberOfFloors)
	: state(id, fromFloor, numberOfFloors) {
	this->state.destinationFloor = randFloor(fromFloor, numberOfFloors);
	this->state.arrivedAt = time(NULL);
}

Person::Person(const Person& copy)
	: state(copy.state) {
}

Person::Person(const PersonState& s)
	: state(s) {
}


unsigned int Person::getArrivalFloor() {
	return state.arrivalFloor;
}

unsigned int Person::getDestinationFloor() {
	return state.destinationFloor;
}

bool Person::travelsUp() {
	return state.arrivalFloor < state.destinationFloor;
}

bool Person::travelsDown() {
	return state.arrivalFloor > state.destinationFloor;
}

int Person::getId() {
	return this->state.id;
}


void Person::startTravel( unsigned int liftId ) {
	this->state.startedTravelAt = time(NULL);
  std::stringstream ss;
  ss << "Person(" << this->getId() << "): ";
	ss << "se tomó el ascensor " << liftId;
	ss << " para viajar desde " << this->getArrivalFloor();
	ss << " hasta " << this->getDestinationFloor();
  log.info(ss.str().c_str());
}

void Person::endTravel( unsigned int liftId ) {
	this->state.endedTravelAt = time(NULL);
  std::stringstream ss;
  ss << "Person(" << this->getId() << "): ";
	ss << "se bajó del ascensor " << liftId <<
  " en el piso " << this->getDestinationFloor();
  log.info(ss.str().c_str());
}

