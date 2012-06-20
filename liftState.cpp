#include "liftState.h"

LiftState::LiftState(unsigned int liftId, unsigned int capacity) {
	this->liftId    = liftId;
	movingDirection = NOT_MOVING;
	totalCapacity   = capacity;
	peopleToGetOff  = peopleRemaining = currentFloor = 0;
}

int LiftState::getMovingDelta() {
	return (int)movingDirection;
}

bool LiftState::isFull() {
	return totalCapacity <= peopleRemaining;
}

bool LiftState::isEmpty() {
	return peopleRemaining <= 0;
}

bool LiftState::isMoving() {
	return movingDirection == UP || movingDirection == DOWN;
}

bool LiftState::hasPeopleToGetOff() {
	return peopleToGetOff > 0;
}

unsigned int LiftState::getPeopleToGetOff() {
	return peopleToGetOff;
}

unsigned int LiftState::getAvailableSpace() {
	return totalCapacity - peopleRemaining;
}

unsigned int LiftState::getLiftId() {
	return liftId;
}

unsigned int LiftState::getCurrentFloor() {
	return currentFloor;
}

MovingDirection LiftState::getMovingDirection() {
	return movingDirection;
}

bool LiftState::goingTo(unsigned int floor) {
	switch(movingDirection) {
		case UP:   return floor > currentFloor;
		case DOWN: return floor < currentFloor;
		default: return false;
	}
}
