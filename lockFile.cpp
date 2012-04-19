#include "lockFile.h"

LockFile :: LockFile ( char* nombre ) {

	strcpy ( this->nombre,nombre );

	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
	this->fl.l_pid = getpid ();
	this->fd = open ( this->nombre,O_CREAT|O_WRONLY,0777 );
	this->closeFd = true;
}

LockFile::LockFile ( int fd ) {
	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
	this->fl.l_pid = getpid ();
	this->fd = fd;
	this->closeFd = false;
}

int LockFile :: tomarLock () {
	this->fl.l_type = F_WRLCK;
	int resultado = fcntl ( this->fd,F_SETLKW,&(this->fl) );
	return resultado;
}

int LockFile :: liberarLock () {
	this->fl.l_type = F_UNLCK;
	int resultado = fcntl ( this->fd,F_SETLK,&(this->fl) );
	return resultado;
}

int LockFile :: escribir ( const char* buffer,int buffsize ) {
	lseek ( this->fd,0,SEEK_END );
	int resultado = write ( this->fd,buffer,buffsize );
	return resultado;
}

LockFile :: ~LockFile () {
  if( this->closeFd ) {
	  close ( this->fd );
  }
}


