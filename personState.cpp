#include "personState.h"
#include <stdlib.h>


PersonState::PersonState(int id, unsigned int fromFloor, unsigned int numberOfFloors) {
	this->id = id;
	this->arrivalFloor = fromFloor;
	this->arrivedAt = time(NULL);
}
PersonState::PersonState(const PersonState& copy) {
	this->id = copy.id;
	this->arrivalFloor     = copy.arrivalFloor;
	this->arrivedAt        = copy.arrivedAt;
	this->startedTravelAt  = copy.startedTravelAt;
	this->endedTravelAt    = copy.endedTravelAt;
	this->destinationFloor = copy.destinationFloor;
}


unsigned int PersonState::getArrivalFloor() {
	return arrivalFloor;
}

unsigned int PersonState::getDestinationFloor() {
	return destinationFloor;
}

int PersonState::getId() {
	return this->id;
}


