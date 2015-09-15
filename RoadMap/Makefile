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

all: RoadMap RoadMapProf RoadMapGProf RoadMapGPerf RoadMap_static RoadMapProf_static RoadMapGProf_static RoadMapGPerf_static RoadMap_dynamic RoadMapProf_dynamic RoadMapGProf_dynamic RoadMapGPerf_dynamic

RoadMap: RoadMap.c
	$(CC) $(CFLAGS) RoadMap.c -o RoadMap $(INC) $(LIB)

RoadMapProf: RoadMap.c
	$(CCP) $(CFLAGS) -pg RoadMap.c -o RoadMapProf $(INC) $(LIB)

RoadMapGProf: RoadMap.c
	$(CC) $(CFLAGS) -pg RoadMap.c -o RoadMapGProf $(INC) $(LIB)

RoadMapGPerf: RoadMap.c
	$(CC) $(CFLAGS) RoadMap.c -o RoadMapGPerf $(INC) $(LIB) -L/state/partition1/apps/lib -lprofiler

RoadMap_static: RoadMap_static.c
	$(CC) $(CFLAGS) RoadMap_static.c -o RoadMap_static $(INC) $(LIB)

RoadMapProf_static: RoadMap_static.c
	$(CCP) $(CFLAGS) -pg RoadMap_static.c -o RoadMapProf_static $(INC) $(LIB)

RoadMapGProf_static: RoadMap_static.c
	$(CC) $(CFLAGS) -pg RoadMap_static.c -o RoadMapGProf_static $(INC) $(LIB)

RoadMapGPerf_static: RoadMap_static.c
	$(CC) $(CFLAGS) RoadMap_static.c -o RoadMapGPerf_static $(INC) $(LIB) -L/state/partition1/apps/lib -lprofiler

RoadMap_dynamic: RoadMap_dynamic.c
	$(CC) $(CFLAGS) RoadMap_dynamic.c -o RoadMap_dynamic $(INC) $(LIB)
	
RoadMapProf_dynamic: RoadMap_dynamic.c
	$(CCP) $(CFLAGS) -pg RoadMap_dynamic.c -o RoadMapProf_dynamic $(INC) $(LIB)

RoadMapGProf_dynamic: RoadMap_dynamic.c
	$(CC) $(CFLAGS) -pg RoadMap_dynamic.c -o RoadMapGProf_dynamic $(INC) $(LIB)

RoadMapGPerf_dynamic: RoadMap_dynamic.c
	$(CC) $(CFLAGS) RoadMap_dynamic.c -o RoadMapGPerf_dynamic $(INC) $(LIB) -L/state/partition1/apps/lib -lprofiler

clean:
	rm -f RoadMap RoadMapProf* results* RoadMap RoadMapProf RoadMapGProf RoadMapGPerf RoadMap_static RoadMapProf_static RoadMapGProf_static RoadMapGPerf_static RoadMap_dynamic RoadMapProf_dynamic RoadMapGProf_dynamic RoadMapGPerf_dynamic gmon.* *~
