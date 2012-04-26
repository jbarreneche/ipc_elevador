#ifndef SET_PUERTAS
#define SET_PUERTAS

#include <iostream>
#include <signal.h>
 
#include "logger.h"

class SetPuertas {
  public:
    SetPuertas(unsigned int cantPuertas);
    ~SetPuertas();

    void agregarPersona( unsigned int numPuerta );
    bool sacarPersona( unsigned int numPuerta );
    int getCantidadDePersonas( unsigned int numPuerta );
    unsigned int getCantidadDePuertas();
    int getGenteEnSistema();
    int waitGenteEnSistema();
  private:

    unsigned int cantPuertas;
    int semId;
    Logger log;

};


#endif
