#include "logger.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <time.h>

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
	this->log( "info ", msg, this->file );
	this->log( "info", msg, this->fileGlobalDebug );
}

void Logger::error( const char* msg ) {
	this->log( "error", msg, this->file );
	this->log( "error", msg, this->fileGlobalDebug );
}

void Logger::debug( const char* msg ) {
  #ifdef LOG_DEBUG
	this->log( "debug", msg, this->fileGlobalDebug );
  #endif
}


void Logger::log( const char* tipoMsg, const char* msg, LockFile& file ) {
  std::stringstream ss;

  char date[MAX_TIMESTAMP_LENGTH];
  time_t timer = time(NULL);

  if( strftime( date, MAX_TIMESTAMP_LENGTH, "%X", localtime(&timer) ) > 0 )
	  ss << date;

  ss << "- " << tipoMsg << "-" << processName << "(pid=" << getpid() << "): " << msg << std::endl;

  std::string msgToPrint;
  msgToPrint=ss.str();

  if( file.getFd() != -1 ) {
	  file.tomarLock();
	  file.escribir( msgToPrint.c_str(), msgToPrint.length() );
	  file.liberarLock();
  }
}


