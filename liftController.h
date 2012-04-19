#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

#include <iostream>

#include <signal.h>

class LiftController {
	public:
		LiftController(int semId);
		int work();
                ~LiftController();

                static void signalHandler( int signum ) {
		  LiftController::continuarSimulacion = 0;
		  std::cout << "SIGINT to LiftController (pid=" << getpid() << ")" << std::endl;
		}

	private:

                static volatile sig_atomic_t continuarSimulacion;

		int semId;
		bool simRunning() {
		  return ( LiftController::continuarSimulacion == 1 );
		}
		void waitGenteEnElSistema();
		void determinarProximoPiso() {}
		void viajarUnPiso() {}
		void bajarPersonas() {}
		void subirPersonas() {}
};



#endif
