CC = mpicc
CCP = mpicc-vt
# Remove -DGRAPHICS to compile with no graphic support (e.g. for measurements)
CFLAGS = -O2 -Wall -g -DGRAPHICS
RRDIR = ../lib

# Adapt this to your distribution if necessary
INCSDL = -I$(RRDIR) `sdl-config --cflags`
LIBSDL = -L/usr/X11R6/lib64 `sdl-config --libs`


INC = $(INCSDL) -I$(RRDIR) 
LIB = $(LIBSDL) -L$(RRDIR) -lX11 -lgraphicsscreen -lSDL -lStopWatch -lpthread -lm

all: RoadMap RoadMapProf RoadMapGProf RoadMapGPerf

FirstMPI: first-mpi.c
	$(CC) $(CFLAGS) first-mpi.c -o FirstMPI $(INC) $(LIB)

FirstMPIStatic: first-mpi-static.c
	$(CC) $(CFLAGS) first-mpi-static.c -o FirstMPIStatic $(INC) $(LIB)

clean:
	rm -f FirstMPI *~
