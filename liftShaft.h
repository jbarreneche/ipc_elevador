#ifndef LIFT_SHAFT
#define LIFT_SHAFT

#include "liftController.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

class LiftShaft {
	public:
		LiftShaft(int semId) : liftController(semId) {
			// initPipes();
		}

		int run() {
			pid_t pid;
			switch (pid = fork()) {
			case -1:
				return -1;
			case 0:
				return liftController.work();
			default:	
				// Timer(); // recibe pipes y parametros de tiempo
				// t.run();
				wait(NULL);
				return 0;
			}
				
		}
	
	private:
		LiftController liftController; // recibe sems y pipes
};

#endif

