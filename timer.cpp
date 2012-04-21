#include "timer.h"

int signalRegister2( int sigNum, void (*handler)(int) );

volatile sig_atomic_t Timer::killPid = 0;

void signalHandler( int signal ) {
  if (Timer::killPid) {
    kill(Timer::killPid, signal);
  }
}

void Timer::start(pid_t killPid) {
  Timer::killPid = killPid;
  std::cout << "I will kill " << Timer::killPid << std::endl;
  signalRegister2( SIGINT, &signalHandler );
}

int signalRegister2( int sigNum, void (*handler)(int) ) {
  struct sigaction sa;

  sa.sa_handler = handler;
  sa.sa_flags = 0; // or SA_RESTART
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sigaction");
    return -1;
  }

  return 0;
}
