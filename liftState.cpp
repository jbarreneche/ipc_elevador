#include "liftState.h"

LiftState::LiftState(unsigned int liftId) {
	this->liftId = liftId;
	availableSpace = peopleToGetOff = 0;
	currentFloor = 0;
	movingDirection = NOT_MOVING;
}

int LiftState::getMovingDelta() {
	return (int)movingDirection;
}

bool LiftState::isFull() {
	return availableSpace == 0;
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
	return availableSpace;
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

void LiftState::getOn() {
	this->peopleToGetOff += 1;
}

void LiftState::getOff() {
	this->peopleToGetOff -= 1;
}
