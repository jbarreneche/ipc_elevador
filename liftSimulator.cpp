#include "liftShaft.h"
#include "peopleGenerator.h"
#include <sys/sem.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int buildSemaphore( int size );
void signalRegister( int sigNum, void (*handler)(int) );

int main() {
  
        // todos los procesos hijo heredan la señal.
        signalRegister( SIGINT, LiftController::signalHandler ); 

	// Iniciar semaforos
	int semId = buildSemaphore( 3 );
	pid_t pid;

	switch (pid = fork()) {
	case -1:

		perror("fork");
		return(-1);

	case 0:
		{
			LiftShaft l(semId); // recibe los semaforos ??
			return l.run();
		}

	default:
		{
			PeopleGenerator g(pid, semId); // recibe los semaforos ??  y TTotal de simulacion
			g.run(30);
			
		}
		wait();
	}
	
	return 0;
}

int buildSemaphore( int size ) {
	key_t key = ftok( "liftSim", 0);
	int semId = semget(key, size, IPC_CREAT | 0666 );

	int data[size] ;

	for(int i = 0; i < size; i++) data[i] = 0;

	semctl(semId, 0, SETALL, data); 

	return semId;
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
