#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

class LiftController {
	public:
		LiftController() {
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

#endif
