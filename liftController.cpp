#include "liftController.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>

LiftController::LiftController(int semId) {
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
