#ifndef LOGGER
#define LOGGER

#include "lockFile.h"

class Logger {
	public:
                Logger( char* pathname );

                //logea a stdout
                Logger();
		
                void info( const char* msg );
                void error( const char* msg );
                void debug( const char* msg );

	private:

                LockFile file;

                void log( const char* tipoMsg, int pid, const char* msg ); 
};



#endif
