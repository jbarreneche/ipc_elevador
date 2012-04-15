#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

class Controller {
	public:
		Controller() {
			// cierra los pipes que no necesita	
			// incializa los sig handlers
		}
		int work() {
			while(simRunning()) {
				waitGenteEnElSistema();
				// proteger sigint, sigterm
				{
					determinarProximoPiso();
					viajarUnPiso();
					// subirOBajarTimer();
					// esperarFinTimer();
					bajarPersonas();
					subirPersonas();	
				}
			}
			return 0;
		}
	private:
		bool simRunning() {
			return true;
		}
		void waitGenteEnElSistema() {}
		void determinarProximoPiso() {}
		void viajarUnPiso() {}
		void bajarPersonas() {}
		void subirPersonas() {}
};


class LiftShaft {
	public:
		LiftShaft() {
			// initPipes();
		}
		int run() {
			int pid;
			switch (pid = fork()) {
			case -1: 
				return -1;
			case 0: 
				{
					Controller c; // recibe sems y pipes
					return c.work();
				}
			default:	
				// Timer(); // recibe pipes y parametros de tiempo
				// t.run();
				wait(NULL);
				return 0;	
			}
				
		}
};

class Generador {
	public:
		Generador(int pid, int time) { }
		void work() {
			sleep(simTime);
		}
	private:
		int simTime;
};

int main() {
	// Iniciar semaforos
	int pid;
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
			Generador g(pid, 30);
			g.work();
			
		}
		wait(NULL);
		exit(0);
		// g Generador(); // recibe los semaforos ??  y TTotal de simulacion
		// g.work();
	}
	
	return 0;
}
