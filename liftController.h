#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

class LiftController {
	public:
		LiftController(int semId);
		int work();

	private:
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
