#include "logger.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdlib.h>

#include <string>
#include <sstream>

#define LOG_DEBUG

int Logger::fdGlobalDebug = -1;

Logger::Logger( char* processName, char* pathName ):
	file(pathName), 
	fileGlobalDebug(Logger::fdGlobalDebug),
	processName(processName)  {
}			       

//logea a stdout
Logger::Logger( const char* processName )
 :file(1), 
	fileGlobalDebug(Logger::fdGlobalDebug),
	processName(processName)  {
}			       

Logger::Logger():
	file(1), 
	fileGlobalDebug(Logger::fdGlobalDebug),
	processName("") {

}			       


void Logger::info( const char* msg ) {
  this->log( "info ", msg );
}

void Logger::error( const char* msg ) {
  this->log( "error", msg );
}

void Logger::debug( const char* msg ) {
  #ifdef LOG_DEBUG
  this->log( "debug", msg );
  #endif
}

void Logger::log( const char* tipoMsg, const char* msg ) {
  std::stringstream ss;
  ss << tipoMsg << "-" << processName << "(pid=" << getpid() << "): " << msg << std::endl;

  std::string msgToPrint;
  msgToPrint=ss.str();

  this->file.tomarLock();
  this->file.escribir( msgToPrint.c_str(), msgToPrint.length() );
  this->file.liberarLock();

	if( Logger::fdGlobalDebug != -1 ) {
		this->fileGlobalDebug.tomarLock();
		this->fileGlobalDebug.escribir( msgToPrint.c_str(), 
																		msgToPrint.length() );
		this->fileGlobalDebug.liberarLock();
	}
}

