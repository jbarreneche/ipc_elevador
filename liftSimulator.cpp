#include "liftShaft.h"
#include "peopleGenerator.h"
#include <sys/sem.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string>

#include "logger.h"

#include <getopt.h>

typedef struct {
  unsigned int cantidadDePuertas;
  unsigned int delayEntrePiso;
  unsigned int tiempoSimulacion;
  unsigned int capacidadAscensor;
	std::string fileDebug;
} Configuracion;

static Configuracion configuracionDefault = {
   2, // Cantidad de Puertas
   10, // Delay entre piso
   10, // Duracion de la simulacion
   1 // Capacidad del ascensor
};

Configuracion parseParams(int argc, char **argv);

int main(int argc, char **argv) {
  srand ( time(NULL) );
  Configuracion configuracion = parseParams(argc, argv);
	
	if ( (configuracion.fileDebug.size() > 0) &&
			 (!Logger::setGlobalDebug( configuracion.fileDebug.c_str() ) ) ) {
		
		std::cout << "Error al abrir el archivo de log :" << 
		configuracion.fileDebug << 
			std::endl;
		
		exit(0);
	}
		
  Logger log;
  SetPuertas puertas(configuracion.cantidadDePuertas);
  pid_t pid;

  log.debug( "start LiftSimulation" );

  switch (pid = fork()) {
    case -1:

      perror("fork");
			Logger::closeGlobalDebug();
      return(-1);

    case 0:
      {
        LiftShaft l(puertas, configuracion.delayEntrePiso, configuracion.capacidadAscensor); // recibe los semaforos ??
        int result = l.run();
				Logger::closeGlobalDebug();
				return result;
      }

    default:
      {
        PeopleGenerator g(puertas); // recibe los semaforos ??  y TTotal de simulacion
        g.run(configuracion.tiempoSimulacion, pid);

      }
      waitpid(pid, NULL, 0);
  }

	Logger::closeGlobalDebug();
  return 0;
}

void showHelp() {
  std::cout << "Usage: liftSim " <<
    " [--help|-h] [--floors|-f=num] [--floor-delay|-d=num]  " <<
    " [--sim-length|-l=num] [--capacity|-c=num]" << 
		" [--output|-o=FILE]" << 
    std::endl;

  std::cout << "\t--help -h\t\tshow this help" << std::endl;
  std::cout << "\t--floors -f=num\t\tset the number of floors to num (has to be greater or equal to 2)" << std::endl;
  std::cout << "\t--floor-delay -d=num\tset the delay between floors to num (has to be greater than 0)" << std::endl;
  std::cout << "\t--sim-length -l=num\tset the length of simulation to num (has to be greater than 0)" << std::endl;
  std::cout << "\t--capacity -c=num\tset the capacity to num (has to be greater than 0)" << std::endl;
  std::cout << "\t--output -o=FILE\tsave log message to the file" << std::endl;
  std::cout << "\t--speed -s=SPEED\tlift moving speed in m/s (changes floor-delay)" << std::endl;
}

Configuracion parseParams(int argc, char **argv) {
  Configuracion configuracion = configuracionDefault;
  struct option long_options[] =
  {
    {"help",        no_argument, 0, 'h'},
    {"floors",      required_argument, 0, 'f'},
    {"floor-delay", required_argument, 0, 'd'},
    {"speed",       required_argument, 0, 's'},
    {"sim-length",  required_argument, 0, 'l'},
    {"capacity",    required_argument, 0, 'c'},
    {"output",      required_argument, 0, 'o'},
    {0, 0, 0, 0}
  };
  int option_index = 0;
  int c = -1;
  int speed = 0;

  while (  (c = getopt_long(argc, argv, "hf:d:l:c:o:", long_options, &option_index)) != -1 ) {
    switch(c) {
      case 'h':
        showHelp();
        exit(0);
      case 'f':
        if(sscanf(optarg, "%u", &configuracion.cantidadDePuertas) <= 0) {
          std::cout << "Invalid floor number: " << optarg << std::endl;
          exit(1);
        }
        if (configuracion.cantidadDePuertas < 2) {
          std::cout << "Invalid floor number (" << configuracion.cantidadDePuertas << ") should be greater than 1" << std::endl;
          exit(2);
        }
        break;
      case 'd':
        if(sscanf(optarg, "%u", &configuracion.delayEntrePiso) <= 0) {
          std::cout << "Invalid floor delay: " << optarg << std::endl;
          exit(1);
        }
        break;
      case 's':
        if(sscanf(optarg, "%u", &speed) <= 0) {
          std::cout << "Invalid floor speed: " << optarg << std::endl;
          exit(1);
        }
        if (speed <= 0) {
          std::cout << "Invalid floor speed: " << optarg << std::endl;
          exit(1);
        }
        configuracion.delayEntrePiso = 3 / speed;
        break;
      case 'l':
        if(sscanf(optarg, "%u", &configuracion.tiempoSimulacion) <= 0) {
          std::cout << "Invalid simulation length: " << optarg << std::endl;
          exit(1);
        }
        break;
      case 'c':
        if(sscanf(optarg, "%u", &configuracion.capacidadAscensor) <= 0) {
          std::cout << "Invalid capacity: " << optarg << std::endl;
          exit(1);
        }
        break;
      case 'o':
				configuracion.fileDebug = optarg;
        break;

      case '?':
        exit(1);
      default:
        showHelp();
        perror("parse");
        exit(1);
    }
  }
  return configuracion;
}
