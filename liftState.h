#ifndef LIFT_STATE_H_
#define LIFT_STATE_H_

enum MovingDirection { DOWN = -1, NOT_MOVING = 0, UP = 1 };

class Lift;

class LiftState {
public:
	LiftState(unsigned int liftId = 0, unsigned int capacity = 0);
	bool isFull();
	bool isEmpty();
	bool isMoving();
	bool hasPeopleToGetOff();
	bool goingTo(unsigned int floor);
	int  getMovingDelta();
	void startMoving(MovingDirection);
	unsigned int getPeopleToGetOff();
	unsigned int getAvailableSpace();
	unsigned int getLiftId();
	unsigned int getCurrentFloor();
	MovingDirection getMovingDirection();

	friend class Lift;

private:
	unsigned int liftId;
	unsigned int totalCapacity;
	unsigned int peopleRemaining;
	unsigned int peopleToGetOff;
    unsigned int currentFloor;
    MovingDirection movingDirection;
};

#endif
