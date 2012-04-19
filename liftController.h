#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

#include <iostream>

#include <vector>
#include <functional>
#include <algorithm>

class LiftController {
	public:
		LiftController(int semId);
		int work();
                ~LiftController();

                void signalHandler( int signum );

                static void callHandlers (int signum) {
		  std::for_each(instances.begin(), 
				instances.end(), 
				std::bind2nd(std::mem_fun(&LiftController::signalHandler),signum));
		}


	private:
                static std::vector<LiftController*> instances;
		int semId;
		bool simRunning() {
			return true;
		}
		void waitGenteEnElSistema();
		void determinarProximoPiso() {}
		void viajarUnPiso() {}
		void bajarPersonas() {}
		void subirPersonas() {}
};


#endif
