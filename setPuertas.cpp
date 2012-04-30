#include "setPuertas.h"

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "logger.h"

SetPuertas::SetPuertas(const SetPuertas &copy) :
  log(copy.log)
{
  creator = false;

  cantPuertas = copy.cantPuertas;
  semId = copy.semId;
  shmId = copy.shmId;
  remapCountersToThisPid();
}

void SetPuertas::remapCountersToThisPid() {
  if ((cantidadPersonas = (unsigned int *)shmat(shmId, NULL, 0)) == (unsigned int*) -1) {
    perror("shmat");
    exit(1);
  }
}

SetPuertas::SetPuertas(unsigned int cantPuertas): log("SetPuertas") {
  struct sembuf sb;
  int semid;

  this->cantPuertas = cantPuertas;
  this->creator = true;
  this->creatorPid = getpid();

  key_t key = ftok("botoneras", 0);
  semid = semget(key, 2, IPC_CREAT | IPC_EXCL | 0666);

  if (semid >= 0) { /* we got it first */
    sb.sem_op = 0; sb.sem_flg = 0;

    for(sb.sem_num = 0; sb.sem_num < 2; sb.sem_num++) {
      /* do a semop() to "free" the semaphores. */
      /* this sets the sem_otime field, as needed below. */
      if (semop(semid, &sb, 1) == -1) {
        int e = errno;
        semctl(semid, 0, IPC_RMID); /* clean up */
        errno = e;
        perror( "creo sem" );
      }
    }

  } else if (errno == EEXIST) { /* someone else got it first */
    semid = semget(key, 2, 0); /* get the id */
    if (semid < 0) 
      perror("ya existen");
  }

  this->semId = semid;
  if ((this->shmId = shmget(key, cantPuertas * sizeof(unsigned int), IPC_CREAT | 0666)) < 0) {
    perror("shmget");
    exit(1);
  }

  log.debug("Inicializando puertas");

  remapCountersToThisPid();
  for( unsigned int i = 0; i < cantPuertas; i ++ ) {
    *(cantidadPersonas + i) = 0;
  }
  unlockSharedMemory();

}

SetPuertas::~SetPuertas(){
  // Soy el set de puertas construido originalmente
  if (creator && this->creatorPid == getpid()) {
    log.debug("Creator dying");
    semctl(this->semId, 0, IPC_RMID);
    shmctl(this->shmId, IPC_RMID, NULL);
  }
}

void SetPuertas::agregarPersona( unsigned int numPuerta ) {
  if( numPuerta >= this->cantPuertas ) {
    log.error( "No existe puerta" );
    return;
  }

  std::stringstream ss;
  ss << "Agrego persona a puerta " << numPuerta;
  log.info(ss.str().c_str());

  // Agregar mutex
  lockSharedMemory();
  (*(cantidadPersonas + numPuerta))++;
  unlockSharedMemory();

  struct sembuf dataop;	
  //actualiza la cantidad total de personas en el sistema
  dataop.sem_num = 1;
  dataop.sem_op =  1;
  dataop.sem_flg = 0;
  semop(this->semId, &dataop, 1);

}

void SetPuertas::unlockSharedMemory() {
  struct sembuf dataop;
  //actualiza la cantidad total de personas en el sistema
  dataop.sem_num = 0;
  dataop.sem_op =  1;
  dataop.sem_flg = 0;
  semop(this->semId, &dataop, 1);
}

void SetPuertas::lockSharedMemory() {
  struct sembuf dataop;
  //actualiza la cantidad total de personas en el sistema
  dataop.sem_num = 0;
  dataop.sem_op =  -1;
  dataop.sem_flg = 0;
  semop(this->semId, &dataop, 1);
}

bool SetPuertas::sacarPersona( unsigned int numPuerta ) {
  if( numPuerta >= this->cantPuertas ) {
    log.error( "No existe puerta" );
    return false;
  }

  struct sembuf dataop;	
  //actualiza la cantidad total de personas en el sistema
  dataop.sem_num = 1;
  dataop.sem_op =  -1;
  dataop.sem_flg = 0;
  semop(this->semId, &dataop, 1);

  lockSharedMemory();
  std::stringstream ss;
  //this->cantidadPersonas = (unsigned int *)shmat(shmId, NULL, 0);
  ss << "Saco persona de puerta " << numPuerta
     << "\nContador: " << (*(cantidadPersonas + numPuerta));
  log.info(ss.str().c_str());

  // Agregar mutex
  (*(cantidadPersonas + numPuerta))--;
  unlockSharedMemory();
  return true;
}

int SetPuertas::waitGenteEnSistema() {
  struct sembuf sb;
  sb.sem_num = 1;
  sb.sem_op = -1;
  sb.sem_flg = 0;
  int result = semop(this->semId, &sb, 1);

  sb.sem_num = 1;
  sb.sem_op = 1;
  sb.sem_flg = 0;
  semop(this->semId, &sb, 1);

  if( result == -1 )
    perror("semop");
  return 0;
}

int SetPuertas::getCantidadDePersonas( unsigned int numPuerta ) {
  lockSharedMemory();
  unsigned int count = (*(cantidadPersonas + numPuerta));
  std::stringstream ss;
  ss << "Cantidad de personas en " << numPuerta << ": "
    << (*(cantidadPersonas + numPuerta));
  log.info(ss.str().c_str());

  unlockSharedMemory();
  return count;
}

unsigned int SetPuertas::getCantidadDePuertas() {
  return this->cantPuertas;
}

