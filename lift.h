#ifndef LIFT
#define LIFT

#include "pipe.h"
#include "liftState.h"
#include "liftMailbox.h"
#include "liftControllerMailbox.h"
#include "logger.h"

#define LIFT_MOVE  'M'
#define LIFT_EXIT  'X'

#define LIFT_OK    'K'

#define DISTANCIA_PISOS   3

class Lift {
  public:
	static volatile sig_atomic_t killPid;
	Lift(unsigned int liftId, unsigned int delayEntrePisos);

	void start(pid_t killPid);
	void travel(MovingDirection);
	void getOn(Person);
	void getOff();
	void end();

  private:
	unsigned int delayEntrePisos;
	bool running;
	LiftMailbox mailbox;
	LiftControllerMailbox controller;
	LiftState state;
	Logger log;
};

#endif
