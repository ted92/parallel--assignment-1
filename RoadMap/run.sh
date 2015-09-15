#!/bin/bash -l

#Replace 5 with the number of hosts to use
NUM_HOSTS=5

if [ "$1" = "profile" ]; then
  echo "localhost" > hostfile
  sh generate_hosts.sh $NUM_HOSTS
  mpirun -x VT_MAX_FLUSHES=50 -x VT_FILTER_SPEC=mpivt_filter -hostfile hostfile RoadMapProf_dynamic
  otfprofile RoadMapProf_dynamic.otf
  pdflatex result.tex
elif [ "$1" = "gprof" ]; then
  echo "localhost" > hostfile
  sh generate_hosts.sh $NUM_HOSTS
  mpirun -hostfile hostfile RoadMapGProf
  gprof -lb RoadMapGProf gmon.*
elif [ "$1" = "gperf" ]; then
  LD_LIBRARY_PATH=/state/partition1/apps/lib:$LD_LIBRARY_PATH CPUPROFILE=profile.out mpirun RoadMapGPerf
    /state/partition1/apps/bin/pprof --lines --text RoadMapGPerf profile.out
else
  echo "localhost" > hostfile
  sh generate_hosts.sh $NUM_HOSTS
  mpirun -hostfile hostfile RoadMap
fi

