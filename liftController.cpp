#include "liftController.h"

#include <stdio.h>
#include <sstream>
#include <stdlib.h>

#include "timer.h"

void signalRegister( int sigNum, void (*handler)(int) );
volatile sig_atomic_t LiftController::continuarSimulacion = 1;

template <class T> const T& min ( const T& a, const T& b ) {
    return (a>b)?b:a;     // or: return comp(a,b)?b:a; for the comp version
}

LiftController::LiftController(SetPuertas puertas, unsigned int capacidad,
			       Pipe* inPipe, Pipe* outPipe ) :
  log("LiftController") ,
  puertas(puertas) ,
  busyFloors(puertas.getCantidadDePuertas(), 0),
  requestedFloors(puertas.getCantidadDePuertas(), 0) {

  numberOfFloors = puertas.getCantidadDePuertas();
  peopleTravelling = 0;
  nextFloor = currentFloor = 0;
  movingDirection = STOPPED;
  lugarDisponible = capacidad;
  // cierra los pipes que no necesita

  this->inPipe = inPipe;
  this->outPipe = outPipe;

  this->inPipe->setearModo(LECTURA);
  this->outPipe->setearModo(ESCRITURA);

}

LiftController::LiftController(SetPuertas puertas, unsigned int capacidad) :
  log("LiftController") ,
  puertas(puertas) ,
  busyFloors(puertas.getCantidadDePuertas(), 0),
  requestedFloors(puertas.getCantidadDePuertas(), 0) {

  numberOfFloors = puertas.getCantidadDePuertas();
  peopleTravelling = 0;
  nextFloor = currentFloor = 0;
  movingDirection = STOPPED;
  lugarDisponible = capacidad;
  // cierra los pipes que no necesita
}

int LiftController::work() {
  char buffer;

  signalRegister( SIGINT, LiftController::signalHandler ); 

  while(simRunning()) {
    waitGenteEnElSistema();
    while ( determinarDireccionDeMovimiento() != STOPPED ) {
      viajarUnPiso();
      bajarPersonas();
      subirPersonas();
    }
  }
  log.info( "Termino el LiftController" );
  log.debug( "Send exit to lift" );
  this->outPipe->escribir( LIFT_EXIT );
  this->inPipe->leer( &buffer, 1 );

  log.debug( "Termino el LiftController ok" );
  return 0;
}

void LiftController::waitGenteEnElSistema() {
	log.info( "Esperando que entre mas gente" );
	puertas.waitGenteEnSistema();
	log.info( "Hay gente!!" );
}

LiftController::~LiftController() {
}

void signalRegister( int sigNum, void (*handler)(int) ) {
  struct sigaction sa;

  sa.sa_handler = handler;
  sa.sa_flags = SA_RESTART;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sigaction");
    exit(0);
  }
}

MovingDirection LiftController::determinarDireccionDeMovimiento() {
  refreshBusyFloors();
  updateMovingDirection();
  if ( movingDirection != STOPPED ) {
    nextFloor = currentFloor + (int)movingDirection;
    std::stringstream ss;
    ss << "Current floor: " << currentFloor << " Next floor: " << nextFloor;
    log.info(ss.str().c_str());
  }
  return movingDirection;
}

void LiftController::refreshBusyFloors() {
  for ( unsigned int i = 0; i < busyFloors.size() ; i ++ ) {
    busyFloors.at(i) = puertas.getCantidadDePersonas( i );
  }
}

void LiftController::updateMovingDirection() {
  int nearestFloor = currentFloor;
  switch ( movingDirection ) {
    case STOPPED: case UP: case NOT_MOVING:
      nearestFloor = findNearestAbove();
      if (nearestFloor == -1) {
        nearestFloor = findNearestBelow();
      }
      break;
    case DOWN:
      nearestFloor = findNearestBelow();
      if (nearestFloor == -1) {
        nearestFloor = findNearestAbove();
      }
  }

  movingDirection = nearestFloor == -1 ?  STOPPED :
    nearestFloor > (int)currentFloor ? UP :
    nearestFloor < (int)currentFloor ? DOWN : NOT_MOVING;
}

void LiftController::viajarUnPiso() {
  char buffer;

  if ( movingDirection != NOT_MOVING ) {
		//sleep(2);
    this->outPipe->escribir(LIFT_MOVE);
    this->inPipe->leer( &buffer, 1 );
    currentFloor = currentFloor + (int)movingDirection;
  }
}

int LiftController::findNearestAbove() {

  int nearest = -1;
  unsigned int currentFloor = this->currentFloor;
  while (nearest == -1 && currentFloor < numberOfFloors ) {
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
  unsigned int currentFloor = this->currentFloor;
  while (nearest == - 1 && currentFloor < numberOfFloors ) {
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
  // Let people get out!!
  if ( requestedFloors.at(currentFloor) > 0 ) {
    log.info("Bajando personas del ascensor");
    lugarDisponible += requestedFloors.at(currentFloor);
    requestedFloors.at(currentFloor) = 0;
  }
}

void LiftController::subirPersonas() {
  // Let people in!!
  if ( !this->isFull() && busyFloors.at(currentFloor) > 0 ) {
    log.info("Subiendo personas!!!");
    int total = min(busyFloors.at(currentFloor), lugarDisponible);
    for ( int i = 0 ; i < total ; i ++ ) {
      puertas.sacarPersona( currentFloor );
      requestedFloors.at(randFloor()) ++ ;
    }
    busyFloors.at(currentFloor) = 0;
    lugarDisponible -= total;
  }
}

bool LiftController::isFull() {
  return ( lugarDisponible == 0 );
}

unsigned int LiftController::randFloor() {
  unsigned int floor = rand() % (numberOfFloors - 1);
  if (floor >= currentFloor) {
    return floor + 1;
  } else {
    return floor;
  }
}
