#include "peopleGenerator.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define DELTA_SLEEP 10
#define MIN_SLEEP 3

PeopleGenerator::PeopleGenerator(pid_t pid) {
  this->pid = pid;
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
  printf( "New born!!" );
}

