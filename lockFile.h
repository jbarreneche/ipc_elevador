#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <fcntl.h>

class LockFile {

private:

	struct flock fl;
	int fd;
	char nombre [ 255 ];
    bool closeFd;

public:

	LockFile ( char* nombre );
	LockFile ( int fd );
	virtual ~LockFile();

	int tomarLock ();
	int liberarLock ();
	int escribir ( const char* buffer, int buffsize );

	int getFd();
};

#endif /* LOCKFILE_H_ */
