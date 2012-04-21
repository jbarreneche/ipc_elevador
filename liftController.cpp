#include "liftController.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>

void signalRegister( int sigNum, void (*handler)(int) );
volatile sig_atomic_t LiftController::continuarSimulacion = 1;

LiftController::LiftController(int semId):log("LiftController") {
	this->semId = semId;
  // cierra los pipes que no necesita
  // incializa los sig handlers
}

int LiftController::work() {
  // todos los procesos hijo heredan la señal.
  signalRegister( SIGINT, LiftController::signalHandler ); 

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

void signalRegister( int sigNum, void (*handler)(int) ) {
  struct sigaction sa;

  sa.sa_handler = handler;
  sa.sa_flags = 0; // or SA_RESTART
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sigaction");
    exit(0);
  }
}
