#ifndef PEOPLE_GENERATOR
#define PEOPLE_GENERATOR

#include <signal.h>
#include <stdlib.h>

class PeopleGenerator {
	public:
		PeopleGenerator(pid_t pid) {
			this->pid = pid;
		}
		void run(int simTime) {
			sleep(simTime);
			kill(pid, SIGINT);
		}
	private:
		pid_t pid;
};

#endif
