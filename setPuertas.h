#ifndef SET_PUERTAS
#define SET_PUERTAS

#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "logger.h"

class SetPuertas {
  public:
    SetPuertas(unsigned int cantPuertas);
    SetPuertas(const SetPuertas &copy);
    ~SetPuertas();

    void agregarPersona( unsigned int numPuerta );
    bool sacarPersona( unsigned int numPuerta );
    int getCantidadDePersonas( unsigned int numPuerta );
    unsigned int getCantidadDePuertas();
    int waitGenteEnSistema();

  private:

    unsigned int cantPuertas;
    int semId;
    int shmId;
    unsigned int *cantidadPersonas;
    bool creator;
    Logger log;
    pid_t creatorPid;

    void remapCountersToThisPid();
    void lockSharedMemory();
    void unlockSharedMemory();

};


#endif
