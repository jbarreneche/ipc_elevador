#include "liftController.h"
#include "lift.h"

#include <sstream>
#include <signal.h>

void signalRegister( int sigNum, void (*handler)(int) );
volatile sig_atomic_t LiftController::continuarSimulacion = 1;

static liftControlledState initialState = {
  0, 0,
  STOPPED
};

template <class T> const T& min ( const T& a, const T& b ) {
    return (a>b)?b:a;     // or: return comp(a,b)?b:a; for the comp version
}

LiftController::LiftController(SetPuertas puertas, unsigned int capacidad,
			       Pipe* inPipe, Pipe* outPipe ) :
  log("LiftController") , puertas(puertas) ,
  liftStates(1, initialState),
  busyFloors(puertas.getCantidadDePuertas(), 0),
  requestedFloors(puertas.getCantidadDePuertas(), 0) {

  peopleTravelling = currentLift = 0;
  numberOfFloors = puertas.getCantidadDePuertas();
  lugarDisponible = capacidad;

  this->inPipe = inPipe;
  this->outPipe = outPipe;

  this->inPipe->setearModo(LECTURA);
  this->outPipe->setearModo(ESCRITURA);

}

int LiftController::work() {
  char buffer;

  signalRegister( SIGINT, LiftController::signalHandler );

  while(simRunning()) {
    waitGenteEnElSistema();
    liftControlledState *liftState = &liftStates[currentLift];
    while ( determinarDireccionDeMovimiento(*liftState) != STOPPED ) {
      // logLiftState(*liftState);
      viajarUnPiso(*liftState);
      bajarPersonas(*liftState);
      subirPersonas(*liftState);
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
  //XXX No hay que cerrar los pipes?!?
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

MovingDirection LiftController::determinarDireccionDeMovimiento(liftControlledState &state) {
  refreshBusyFloors();
  updateMovingDirection(state);
  if ( state.movingDirection != STOPPED ) {
    state.nextFloor = state.currentFloor + (int)state.movingDirection;
    std::stringstream ss;
    ss << "Current floor: " << state.currentFloor << " Next floor: " << state.nextFloor;
    log.info(ss.str().c_str());
  }
  return state.movingDirection;
}

void LiftController::refreshBusyFloors() {
  for ( unsigned int i = 0; i < busyFloors.size() ; i ++ ) {
    busyFloors.at(i) = puertas.getCantidadDePersonas( i );
  }
}

void LiftController::updateMovingDirection(liftControlledState &state) {
  int nearestFloor = state.currentFloor;
  switch ( state.movingDirection ) {
    case STOPPED: case UP: case NOT_MOVING:
      nearestFloor = findNearestAbove(state);
      if (nearestFloor == -1) {
        nearestFloor = findNearestBelow(state);
      }
      break;
    case DOWN:
      nearestFloor = findNearestBelow(state);
      if (nearestFloor == -1) {
        nearestFloor = findNearestAbove(state);
      }
  }

  state.movingDirection = nearestFloor == -1 ?  STOPPED :
    nearestFloor > (int)state.currentFloor ? UP :
    nearestFloor < (int)state.currentFloor ? DOWN : NOT_MOVING;
}

void LiftController::viajarUnPiso(liftControlledState &state) {
  char buffer;

  if ( state.movingDirection != NOT_MOVING ) {
    this->outPipe->escribir(LIFT_MOVE);
    this->inPipe->leer( &buffer, 1 );
    state.currentFloor = state.currentFloor + (int)state.movingDirection;
  }
}

int LiftController::findNearestAbove(liftControlledState &state) {

  int nearest = -1;
  unsigned int currentFloor = state.currentFloor;
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

int LiftController::findNearestBelow(liftControlledState &state) {
  int nearest = - 1;
  unsigned int currentFloor = state.currentFloor;
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

void LiftController::bajarPersonas(liftControlledState &state) {
  // Let people get out!!
  if ( requestedFloors.at(state.currentFloor) > 0 ) {
    log.info("Bajando personas del ascensor");
    lugarDisponible += requestedFloors.at(state.currentFloor);
    requestedFloors.at(state.currentFloor) = 0;
  }
}

void LiftController::subirPersonas(liftControlledState &state) {
  // Let people in!!
  if ( !this->isFull() && busyFloors.at(state.currentFloor) > 0 ) {
    log.info("Subiendo personas!!!");
    int total = min(busyFloors.at(state.currentFloor), lugarDisponible);
    for ( int i = 0 ; i < total ; i ++ ) {
      puertas.sacarPersona( state.currentFloor );
      requestedFloors.at(randFloor(state.currentFloor)) ++ ;
    }
    busyFloors.at(state.currentFloor) = 0;
    lugarDisponible -= total;
  }
}

bool LiftController::isFull() {
  return ( lugarDisponible == 0 );
}

unsigned int LiftController::randFloor(unsigned int excludeFloor) {
  unsigned int floor = rand() % (numberOfFloors - 1);
  if (floor >= excludeFloor) {
    return floor + 1;
  } else {
    return floor;
  }
}

void LiftController::logLiftState(liftControlledState state) {
  std::stringstream ss;
  ss << "Lift(1) state: (CF: " << state.currentFloor << " NF:" << state.nextFloor << " MD:" << state.movingDirection << ")";
  log.info(ss.str().c_str());
}