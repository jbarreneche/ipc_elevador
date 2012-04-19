#include "liftController.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>

volatile sig_atomic_t LiftController::continuarSimulacion = 1;

LiftController::LiftController(int semId):log("LiftController") {
	this->semId = semId;
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
  log.info( "Termino el LiftController" );

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
	log.info( "Hay gente!!" );
}


LiftController::~LiftController() {
}




