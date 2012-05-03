#ifndef LOGGER
#define LOGGER

#include "lockFile.h"
#include <string>
#include <iostream>

class Logger {
	public:

                Logger( char* processName, char* pathname );
                //logea a stdout
                Logger( const char* processName );
                Logger();
		
                void info( const char* msg );
                void error( const char* msg );
                void debug( const char* msg );

	              static bool setGlobalDebug(const char* file) {
									if( fdGlobalDebug == -1 ) {
										Logger::fdGlobalDebug = open ( file,O_CREAT|O_WRONLY,0777 );
										return( Logger::fdGlobalDebug >= 0 );
									}
									return false;
								}

	              static void closeGlobalDebug() {
									close(fdGlobalDebug);
									fdGlobalDebug = -1;
								}

	private:
	              static int fdGlobalDebug;

                LockFile file;
                LockFile fileGlobalDebug;
                std::string processName;  
                void log( const char* tipoMsg, const char* msg ); 
};



#endif
