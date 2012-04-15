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
		// separar proceso controlador
		{
			LiftShaft l;
			exit(l.run());
		}
		// c Controller(); // recibe los semaforos ??
		// exit(c.work());
	default:
		// separar proceso generador
		{
			PeopleGenerator g(pid);
			g.run(30);
			
		}
		wait(NULL);
		exit(0);
		// g Generador(); // recibe los semaforos ??  y TTotal de simulacion
		// g.work();
	}
	
	return 0;
}

