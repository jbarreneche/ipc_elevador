#ifndef LIFT
#define LIFT

#include "pipe.h"
#include "logger.h"

#define LIFT_MOVE  'M'
#define LIFT_EXIT  'X'

#define LIFT_OK    'K'

#define DISTANCIA_PISOS   3

class Lift {
  public:
	Lift( unsigned int delayEntrePisos, Pipe* inPipe, Pipe* outPipe );

	void start(pid_t killPid);
	static volatile sig_atomic_t killPid;

  private:
	unsigned int delayEntrePisos;
	Pipe *inPipe, *outPipe;
	Logger log;
};

#endif
