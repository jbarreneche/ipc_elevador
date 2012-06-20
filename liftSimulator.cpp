#include "logger.h"
#include "liftShaft.h"
#include "peopleGenerator.h"
#include "setPuertas.h"

#include <errno.h>
#include <iostream>

#include <getopt.h>

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct Configuracion {
  unsigned int cantidadDePuertas;
  unsigned int delayEntrePiso;
  unsigned int tiempoSimulacion;
  unsigned int capacidadAscensor;
  unsigned int tiempoEntrePersona;
	std::string fileDebug;
  Configuracion() :
    cantidadDePuertas(5),
    delayEntrePiso(5),
    tiempoSimulacion(20),
    capacidadAscensor(4),
    tiempoEntrePersona(10) {};
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

  Logger log; pid_t pid; int status;
  SetPuertas puertas(configuracion.cantidadDePuertas);

  log.debug( "start LiftSimulation" );

  switch (pid = fork()) {
    case -1:

      perror("fork");
			Logger::closeGlobalDebug();
      return(-1);

    case 0: {
      LiftShaft shaft(puertas, configuracion.delayEntrePiso, configuracion.capacidadAscensor);
      status = shaft.run();
			Logger::closeGlobalDebug();
			return status;
    }

    default: {
      PeopleGenerator generador(configuracion.tiempoEntrePersona, configuracion.cantidadDePuertas);
      generador.run(configuracion.tiempoSimulacion, pid);

      waitpid(pid, &status, 0);
    }
  }

	Logger::closeGlobalDebug();
  return status;
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
  std::cout << "\t--gen-delay -g=num\tset the maximum delay between each generated person (has to be greater than 0)" << std::endl;
  std::cout << "\t--output -o=FILE\tsave log message to the file" << std::endl;
  std::cout << "\t--speed -s=SPEED\tlift moving speed in m/s (changes floor-delay)" << std::endl;
}

Configuracion parseParams(int argc, char **argv) {
  Configuracion configuracion;
  struct option long_options[] =
  {
    {"help",        no_argument, 0, 'h'},
    {"floors",      required_argument, 0, 'f'},
    {"floor-delay", required_argument, 0, 'd'},
    {"speed",       required_argument, 0, 's'},
    {"sim-length",  required_argument, 0, 'l'},
    {"capacity",    required_argument, 0, 'c'},
    {"gen-delay",   required_argument, 0, 'g'},
    {"output",      required_argument, 0, 'o'},
    {0, 0, 0, 0}
  };
  int option_index = 0;
  int c = -1;
  int speed = 0;

  while (  (c = getopt_long(argc, argv, "hf:d:l:c:o:g:", long_options, &option_index)) != -1 ) {
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
      case 'g':
        if(sscanf(optarg, "%u", &configuracion.tiempoEntrePersona) <= 0) {
          std::cout << "Invalid generator delay: " << optarg << std::endl;
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
