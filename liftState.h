#ifndef LIFT_STATE_H_
#define LIFT_STATE_H_

enum MovingDirection { DOWN = -1, NOT_MOVING = 0, UP = 1 };

class Lift;

class LiftState {
public:
	LiftState(unsigned int liftId);
	bool isFull();
	bool isMoving();
	bool hasPeopleToGetOff();
	bool goingTo(unsigned int floor);
	int  getMovingDelta();
	unsigned int getPeopleToGetOff();
	unsigned int getAvailableSpace();
	unsigned int getLiftId();
	unsigned int getCurrentFloor();
	MovingDirection getMovingDirection();

	void getOn();
	void getOff();

	friend class Lift;

private:
	unsigned int liftId;
	unsigned int availableSpace;
	unsigned int peopleToGetOff;
    unsigned int currentFloor;
    MovingDirection movingDirection;
};

#endif
