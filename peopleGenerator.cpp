#include "peopleGenerator.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/sem.h>

#define DELTA_SLEEP 10
#define MIN_SLEEP 3

PeopleGenerator::PeopleGenerator(pid_t pid, int semId) {
  this->pid = pid;
	this->semId = semId;
}

void PeopleGenerator::run(int simTime) {
  time_t start = time(NULL);
  time_t end = start + simTime;

  while( time(NULL) < end ) {
    sleep( rand() % DELTA_SLEEP + MIN_SLEEP );
    spawnPerson();
  }

  kill(pid, SIGINT);
}

void PeopleGenerator::spawnPerson() {
  printf( "New born!!\n" );
	// key_t key = ftok( "liftSim", 0);
	// int semId = semget(key, 3, 0);
	struct sembuf dataop;	
	dataop.sem_num = 1;
	dataop.sem_op =  1;
	dataop.sem_flg = 0;

	semop(semId, &dataop, 1);
}

