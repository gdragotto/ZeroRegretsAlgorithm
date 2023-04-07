#!/bin/bash
  instanceFolderSa=../instance_generators/instances_quad/generated
  resultsFolder=../dat
  numThreads=8
  max=0
  logFile=${resultsFolder}/resultsQuadSa${max}.csv
  timelimit=1800
  mv bin/TheQuadraticGame bin/R_TheQuadraticGame

  #Sagratella's instances
  for n in 2 3 4 6 #10 #20
  do
      for m in 1 2 3 5 10
      do
            instanceFile=${instanceFolderSa}/Quadratic-${n}-${m}-A-H
            if test -f "$instanceFile.txt"; then
              instanceName=${instanceFile/.txt/}
              printf "####################### ${instanceName} #######################\n"
              ./bin/R_TheQuadraticGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 1 --max ${max} --nonv 1 --time ${timelimit}
              printf "\\n\\n"xw
            fi
            instanceFile=${instanceFolderSa}/Quadratic-${n}-${m}-B-H
            if test -f "$instanceFile.txt"; then
              instanceName=${instanceFile/.txt/}
              printf "####################### ${instanceName} #######################\n"
              ./bin/R_TheQuadraticGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 1 --max ${max} --nonv 1 --time ${timelimit}
              printf "\\n\\n"xw
            fi

            instanceFile=${instanceFolderSa}/Quadratic-${n}-${m}-A-L
            if test -f "$instanceFile.txt"; then
              instanceName=${instanceFile/.txt/}
              printf "####################### ${instanceName} #######################\n"
              ./bin/R_TheQuadraticGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 1 --max ${max} --nonv 1 --time ${timelimit}
              printf "\\n\\n"
            fi
            instanceFile=${instanceFolderSa}/Quadratic-${n}-${m}-B-L
            if test -f "$instanceFile.txt"; then
              instanceName=${instanceFile/.txt/}
              printf "####################### ${instanceName} #######################\n"
              ./bin/R_TheQuadraticGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 1 --max ${max} --nonv 1 --time ${timelimit}
              printf "\\n\\n"
            fi

      done
  done




