#include "logger.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdlib.h>

#include <string>
#include <sstream>

#define LOG_DEBUG

Logger::Logger( char* pathName ):file(pathName) {
}			       

//logea a stdout
Logger::Logger():file(1) {
}			       


void Logger::info( const char* msg ) {
  this->log( "INFO", getpid(), msg );
}

void Logger::error( const char* msg ) {
  this->log( "ERROR", getpid(), msg );
}

void Logger::debug( const char* msg ) {
  #ifdef LOG_DEBUG
  this->log( "DEBUG", getpid(), msg );
  #endif
}

void Logger::log( const char* tipoMsg, int pid, const char* msg ) {
  std::stringstream ss;
  ss << tipoMsg << "(pid=" << pid << "): " << msg << std::endl;

  std::string msgToPrint;
  msgToPrint=ss.str();

  this->file.tomarLock();
  this->file.escribir( msgToPrint.c_str(), msgToPrint.length() );
  this->file.liberarLock();
}






