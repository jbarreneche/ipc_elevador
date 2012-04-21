#include "liftController.h"

#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <sys/sem.h>

void signalRegister( int sigNum, void (*handler)(int) );
volatile sig_atomic_t LiftController::continuarSimulacion = 1;

LiftController::LiftController(int semId, int numberOfFloors) : log("LiftController") , busyFloors(numberOfFloors, 0), requestedFloors(numberOfFloors, 0) {
  this->semId = semId;
  this->numberOfFloors = numberOfFloors;
  peopleTravelling = 0;
  nextFloor = currentFloor = 0;
  movingDirection = NOT_MOVING;
  busyFloors.at(5) = 1;
  busyFloors.at(0) = 1;
  // cierra los pipes que no necesita
}

int LiftController::work() {
  // todos los procesos hijo heredan la señal.
  signalRegister( SIGINT, LiftController::signalHandler ); 

  while(simRunning()) {
    waitGenteEnElSistema();
    determinarProximoPiso();
    while ( movingDirection != NOT_MOVING) {
      viajarUnPiso();
      // subirOBajarTimer();
      // esperarFinTimer();
      bajarPersonas();
      subirPersonas();
      determinarProximoPiso();
    }
  }
  log.info( "Termino el LiftController" );

  return 0;
}

void LiftController::waitGenteEnElSistema() {
	struct sembuf dataop;	
	dataop.sem_num = 1;
	dataop.sem_op = - (peopleTravelling + 1);
	dataop.sem_flg = 0;

	log.info( "Esperando que entre mas gente" );
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

void LiftController::determinarProximoPiso() {
  refreshBusyFloors();
  updateMovingDirection();
  nextFloor = currentFloor + (int)movingDirection;
  std::stringstream ss;
  ss << "Current floor: " << currentFloor << " Next floor: " << nextFloor;
  log.debug(ss.str().c_str());
}

void LiftController::updateMovingDirection() {
  int nearestFloor = currentFloor;
  switch ( movingDirection ) {
    case NOT_MOVING: case UP:
      nearestFloor = findNearestAbove();
      if (nearestFloor == -1) {
        nearestFloor = findNearestBelow();
      }
      //nearestFloor;
      break;
    case DOWN:
      nearestFloor = findNearestBelow();
      if (nearestFloor == -1) {
        nearestFloor = findNearestAbove();
      }
  }

  movingDirection = nearestFloor == -1 ?  NOT_MOVING :
    nearestFloor > (int)currentFloor ? UP :
    nearestFloor < (int)currentFloor ? DOWN : NOT_MOVING;
}

void LiftController::viajarUnPiso() {
  currentFloor = nextFloor;
}

int LiftController::findNearestAbove() {
  int nearest = -1;
  unsigned int currentFloor = this->currentFloor + 1;
  while (nearest == -1 && currentFloor < requestedFloors.size() ) {
    if ( requestedFloors.at(currentFloor) > 0 || 
        (busyFloors.at(currentFloor) > 0 && !this->isFull()) ) {
      nearest = currentFloor;
      break;
    }
    currentFloor++;
  }
  return nearest;
}

int LiftController::findNearestBelow() {
  int nearest = - 1;
  unsigned int currentFloor = this->currentFloor - 1;
  while (nearest == - 1 && currentFloor < requestedFloors.size() ) {
    if ( requestedFloors.at(currentFloor) > 0 || 
        (busyFloors.at(currentFloor) > 0 && !this->isFull()) ) {
      nearest = currentFloor;
      break;
    }
    currentFloor--;
  }
  return nearest;
}

void LiftController::bajarPersonas() {
  // Let people in!!
  if ( requestedFloors.at(currentFloor) > 0 ) {
    log.info("Bajando personas del ascensor");
    requestedFloors.at(currentFloor) = 0;
  }
}

void LiftController::subirPersonas() {
  // Let people in!!
  if ( !this->isFull() && busyFloors.at(currentFloor) > 0 ) {
    log.info("Subiendo personas!!!");
    busyFloors.at(currentFloor) = 0;
  }
}

bool LiftController::isFull() {
  return false;
}
