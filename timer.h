#ifndef TIMER
#define TIMER
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class Timer {
  public:
    Timer(int _speed) { speed = _speed; };
    void start(pid_t killPid);
    static volatile sig_atomic_t killPid;

  private:
    int speed;
};

#endif
