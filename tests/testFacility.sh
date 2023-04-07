#!/bin/bash
  instanceFolder=../instance_generators/instances_cld/
  resultsFolder=../dat
  logFile=${resultsFolder}/resultsFacility.csv
  numThreads=8
  timelimit=3600
  mv bin/TheFacilityGame bin/R_TheFacilityGame
    for dmax in 20 #40
    do
      for beta in 0.5 #2 3
      do
        for b1 in 10 20 30 40
        do
          for b2 in 10 20 30 40 50 60 70 80 90 100
          do
            if [ "$b2" -ge "$b1" ]; then

              printf "####################### n=2 beta=${beta} Capacities:${b1}-${b2}  #######################\n"
              ./bin/R_TheFacilityGame -f ${instanceFolder} --threads ${numThreads} --log ${logFile} --enumerate 0  --time ${timelimit} --n 2 --beta ${beta} --B1 ${b1} --B2 ${b2} --dmax ${dmax}
              printf "\\n\\n"


              printf "####################### n=3 beta=${beta} Capacities:${b1}-${b2}-${b3}  #######################\n"
              ./bin/R_TheFacilityGame -f ${instanceFolder} --threads ${numThreads} --log ${logFile} --enumerate 0  --time ${timelimit} --n 3 --beta ${beta} --B1 ${b1} --B2 ${b2} --B3 10 --dmax ${dmax}
              printf "\\n\\n"
              if [ "$b1" -gt 39 ]  && [ "$b2" -gt 79 ]; then
                  exit 0
              fi

            fi
        done
      done
    done
  done
