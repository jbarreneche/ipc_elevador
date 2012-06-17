#ifndef PIPE_H_
#define PIPE_H_

#define	LECTURA		0
#define	ESCRITURA	1

class Pipe {

private:

	int descriptores[2];
	bool lectura;
	bool escritura;

public:
	Pipe();
	virtual ~Pipe();

	void setearModo ( int modo );

	int escribir ( char* dato,int datoSize );
	int escribir ( const char dato );
	int leer ( char* buffer,int buffSize );

private:
	int getFdLectura ();
	int getFdEscritura ();

	void cerrar ();
};

#endif /* PIPE_H_ */
