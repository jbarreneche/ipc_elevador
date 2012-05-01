#ifndef TIMER
#define TIMER
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "pipe.h"
#include "logger.h"

#define LIFT_MOVE  'M'
#define LIFT_EXIT  'X'

#define LIFT_OK    'K'

class Timer {
  public:
	Timer( int _speed, Pipe* inPipe, Pipe* outPipe );

	void start(pid_t killPid);
	static volatile sig_atomic_t killPid;

private:
	int speed;
	Pipe* inPipe;
	Pipe* outPipe;
	Logger log;
};

#endif
