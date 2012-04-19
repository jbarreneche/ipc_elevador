#ifndef LOGGER
#define LOGGER

#include "lockFile.h"
#include <string>

class Logger {
	public:

                Logger( char* processName, char* pathname );
                //logea a stdout
                Logger( const char* processName );
                Logger();
		
                void info( const char* msg );
                void error( const char* msg );
                void debug( const char* msg );

	private:

                LockFile file;
                std::string processName;  
                void log( const char* tipoMsg, const char* msg ); 
};



#endif
