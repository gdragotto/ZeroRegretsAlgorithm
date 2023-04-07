#!/bin/bash
  instanceFolderNC=../instance_generators/instances_quad/non_convex
  instanceFolderC=../instance_generators/instances_quad/convex
  resultsFolder=../dat
  logFile=${resultsFolder}/resultsQuadSS.csv
  numThreads=8
  timelimit=1800
  mv bin/TheQuadraticGame bin/R_TheQuadraticGame

for instanceFile in `find ${instanceFolderC} -name "*.txt" -type f`; do
  instanceName=${instanceFile/.txt/}
  printf "####################### ${instanceName} #######################\n"
  ./bin/R_TheQuadraticGame -f ${instanceName} -t ${numThreads} --log ${logFile} --enumerate 1 --max 0 --nonv 1 --time ${timelimit}
  printf "\\n\\n"
done

for instanceFile in `find ${instanceFolderNC} -name "*.txt" -type f`; do
  instanceName=${instanceFile/.txt/}
  printf "####################### ${instanceName} #######################\n"
  ./bin/R_TheQuadraticGame -f ${instanceName} -t ${numThreads} --log ${logFile} --enumerate 1 --max 0 --nonv 1 --time ${timelimit}
  printf "\\n\\n"
done




