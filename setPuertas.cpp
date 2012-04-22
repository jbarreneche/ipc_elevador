#include "setPuertas.h"

#include <sys/sem.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "logger.h"

SetPuertas::SetPuertas(int cantPuertas): log("SetPuertas") {
  struct sembuf sb;
  int semid;

  this->cantPuertas = cantPuertas;

  key_t key = ftok("botoneras", 0);  
  semid = semget(key, cantPuertas+1, IPC_CREAT | IPC_EXCL | 0666);

  if (semid >= 0) { /* we got it first */
    sb.sem_op = 0; sb.sem_flg = 0;

    for(sb.sem_num = 0; sb.sem_num < cantPuertas+1; sb.sem_num++) { 
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
    semid = semget(key, cantPuertas+1, 0); /* get the id */
    if (semid < 0) 
      perror("ya existen");
  }

  this->semId = semid;

}

SetPuertas::~SetPuertas(){
  semctl(this->semId, 0, IPC_RMID);
}

void SetPuertas::agregarPersona( int numPuerta ) {
  if( numPuerta >= this->cantPuertas ) {
    log.error( "No existe puerta" );
    return;
  }

  //actualiza la cantidad de personas en la puerta
  struct sembuf dataop;	
  dataop.sem_num = numPuerta;
  dataop.sem_op =  1;
  dataop.sem_flg = 0;
  semop(this->semId, &dataop, 1);

  //actualiza la cantidad total de personas en el sistema
  dataop.sem_num = this->cantPuertas;
  dataop.sem_op =  1;
  dataop.sem_flg = 0;
  semop(this->semId, &dataop, 1);


  std::stringstream ss;
  ss << "Agrego persona a puerta " << numPuerta;
  log.info(ss.str().c_str());
}

bool SetPuertas::sacarPersona( int numPuerta ) {
  if( numPuerta >= this->cantPuertas ) {
    log.error( "No existe puerta" );
    return false;
  }

  //actualiza la cantidad de personas en la puerta
  struct sembuf dataop;	
  dataop.sem_num = numPuerta;
  dataop.sem_op =  -1;
  dataop.sem_flg = 0;
  semop(this->semId, &dataop, 1);

  //actualiza la cantidad total de personas en el sistema
  dataop.sem_num = this->cantPuertas;
  dataop.sem_op =  -1;
  dataop.sem_flg = 0;
  semop(this->semId, &dataop, 1);

  std::stringstream ss;
  ss << "Saco persona de puerta " << numPuerta;
  log.info(ss.str().c_str());

  return true;
}

int SetPuertas::waitGenteEnSistema() {
  struct sembuf sb;
  sb.sem_num = this->cantPuertas;
  sb.sem_op = -1;
  sb.sem_flg = 0;  
  int result = semop(this->semId, &sb, 1);

  sb.sem_num = this->cantPuertas;
  sb.sem_op = 1;
  sb.sem_flg = 0;
  semop(this->semId, &sb, 1);

  if( result == -1 )
    perror("semop");
  return 0;
}

int SetPuertas::getGenteEnSistema() {
  int count;
  count = semctl(this->semId, this->cantPuertas, GETVAL );
  return count;
}

int SetPuertas::getCantidadDePersonas( int numPuerta ) {
  int count = semctl(this->semId, numPuerta, GETVAL );

  //std::stringstream ss;
  //ss << "Cantidad de personas en " << numPuerta << ": " << count;
  //log.info(ss.str().c_str());

  return count;
}

int SetPuertas::getCantidadDePuertas() {
  return this->cantPuertas;
}




