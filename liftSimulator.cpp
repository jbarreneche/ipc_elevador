#include "liftShaft.h"
#include "peopleGenerator.h"

int main() {

	// Iniciar semaforos
	pid_t pid;

	switch (pid = fork()) {
	case -1:

		perror("fork");
		exit(-1);

	case 0:
		{
			LiftShaft l; // recibe los semaforos ??
			exit(l.run());
		}

	default:
		{
			PeopleGenerator g(pid); // recibe los semaforos ??  y TTotal de simulacion
			g.run(30);
			
		}
		wait(NULL);
	}
	
	return 0;
}

