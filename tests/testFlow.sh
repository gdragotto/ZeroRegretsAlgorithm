#!/bin/bash
  instanceFolderNC=../instance_generators/instances_cdf/converted
  resultsFolder=../dat
  logFile=${resultsFolder}/resultsFlowContinuous.csv
  numThreads=8
  timelimit=1800
  mv bin/TheFlowGame bin/R_TheFlowGame

for instanceFile in `find ${instanceFolderNC} -name "*.txt" -type f`; do
  instanceName=${instanceFile/.txt/}
  printf "####################### ${instanceName} #######################\n"
  ./bin/R_TheFlowGame -f ${instanceName} -t ${numThreads} --log ${logFile} --enumerate 0 --max 0 --time ${timelimit}
  printf "\\n\\n"
done


