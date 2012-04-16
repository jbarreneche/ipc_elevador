#ifndef LIFT_CONTROLLER
#define LIFT_CONTROLLER

class LiftController {
	public:
		LiftController();
		int work();

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
