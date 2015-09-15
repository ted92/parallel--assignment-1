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

RoadMap: RoadMap.c
	$(CC) $(CFLAGS) RoadMap.c -o RoadMap $(INC) $(LIB)

RoadMap_staticGProf: RoadMap_static.c
	$(CC) $(CFLAGS) -pg RoadMap_static.c -o RoadMap_staticGProf $(INC) $(LIB)

RoadMap_static: RoadMap_static.c AASDASDASADSKSK
	$(CC) $(CFLAGS) RoadMap_static.c -o RoadMap_static $(INC) $(LIB)

RoadMap_static2: RoadMap_static2.c
	$(CC) $(CFLAGS) RoadMap_static2.c -o RoadMap_static2 $(INC) $(LIB)

RoadMap_dynamic: RoadMap_dynamic.c
	$(CC) $(CFLAGS) RoadMap_dynamic.c -o RoadMap_dynamic $(INC) $(LIB)

RoadMapProf: RoadMap.c
	$(CCP) $(CFLAGS) -pg RoadMap.c -o RoadMapProf $(INC) $(LIB)
	
RoadMapProf_dynamic: RoadMap_dynamic.c
	$(CCP) $(CFLAGS) -pg RoadMap_dynamic.c -o RoadMapProf_dynamic $(INC) $(LIB)

RoadMapProf_static: RoadMap_static.c
	$(CCP) $(CFLAGS) -pg RoadMap_static.c -o RoadMapProf_static $(INC) $(LIB)
	
RoadMapGProf: RoadMap.c
	$(CC) $(CFLAGS) -pg RoadMap.c -o RoadMapGProf $(INC) $(LIB)

RoadMapGPerf: RoadMap.c
	        $(CC) $(CFLAGS) RoadMap.c -o RoadMapGPerf $(INC) $(LIB) -L/state/partition1/apps/lib -lprofiler

clean:
	rm -f RoadMap RoadMapProf* results* RoadMapGProf RoadMapGPerf RoadMap_dynamic RoadMap_static2 RoadMap_static RoadMap_staticGProf RoadMapProf_static RoadMapProf_dynamic gmon.* *~
