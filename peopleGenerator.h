#ifndef PEOPLE_GENERATOR
#define PEOPLE_GENERATOR

#include <signal.h>

#include "logger.h"
class PeopleGenerator {
	public:

		PeopleGenerator(pid_t pid, int semId);
		void run(int simTime);

	private:
		pid_t pid;
		int semId;

                Logger log;
		void spawnPerson();
};

#endif
