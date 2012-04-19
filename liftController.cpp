#include "liftController.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>

//inicializo el atributo estatico.
std::vector<LiftController*> LiftController::instances;

LiftController::LiftController(int semId) {
	this->semId = semId;
	LiftController::instances.push_back(this);
  // cierra los pipes que no necesita
  // incializa los sig handlers
}

int LiftController::work() {
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

void LiftController::waitGenteEnElSistema() {
	// key_t key = ftok( "liftSim", 0);
	// int semId = semget(key, 3, 0);
	struct sembuf dataop;	
	dataop.sem_num = 1;
	dataop.sem_op = -1;
	dataop.sem_flg = 0;

	semop(semId, &dataop, 1);
	printf("Hay genteeeeeee!!!\n");
}

void LiftController::signalHandler( int signum ) {
  std::cout << "SIGINT to LiftController (pid=" << getpid() << ")" << std::endl;
}



LiftController::~LiftController() {
  std::vector<LiftController*>::iterator where = std::find( instances.begin(), 
							    instances.end(), 
							    this );
  instances.erase(where);
}




