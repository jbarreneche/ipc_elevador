CC = g++
CFLAGS = -Wall -std=c++98 -ggdb
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXE_SIM = liftSim
LDFLAGS =

all: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXE_SIM)

.cpp.o:
	g++ -c $(CFLAGS) $<

debug:
	strace -a 59 -o trace -ff -tt -e trace=ipc,read,write,open,close,fork,process $(EXE_SIM)

clean:
	rm -f *.o

cleanall: $(clean)
	rm -f $(EXE_SIM)
