#ifndef SET_PUERTAS
#define SET_PUERTAS

#include <iostream>
#include <signal.h>
 
#include "logger.h"

class SetPuertas {
  public:
    SetPuertas(int cantPuertas);
    ~SetPuertas();

    void agregarPersona( int numPuerta );
    bool sacarPersona( int numPuerta );
    int getCantidadDePersonas( int numPuerta );
    int getCantidadDePuertas();
    int getGenteEnSistema();
    int waitGenteEnSistema();
  private:

    int cantPuertas;
    int semId;
    Logger log;

};    


#endif
