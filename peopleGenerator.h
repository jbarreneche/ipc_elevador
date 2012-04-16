#ifndef PEOPLE_GENERATOR
#define PEOPLE_GENERATOR

#include <signal.h>

class PeopleGenerator {
	public:

		PeopleGenerator(pid_t pid);
		void run(int simTime);

	private:
		pid_t pid;

		void spawnPerson();
};

#endif
