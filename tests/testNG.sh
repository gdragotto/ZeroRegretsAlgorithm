#!/bin/bash
  instanceFolder=../instance_generators/instances_ng/generated
  resultsFolder=../dat/
  logFile=${resultsFolder}/resultsNG.csv
  numThreads=8
  timelimit=1800
  mv bin/TheNetworkGame bin/R_TheNetworkGame
  for n in 50 100 150 200 250 300 350 400 450 500
  do
            instanceFile=${instanceFolder}/G_n_${n}-grid.adj
            if test -f "$instanceFile"; then
              instanceName=${instanceFile/.txt/}
              printf "####################### ${instanceName} #######################\n"
                ./bin/R_TheNetworkGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 0 --max 0 --stability 1 --dsink 1 --alpha1 0.33 --alpha2 0.33 --time ${timelimit}
                ./bin/R_TheNetworkGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 0 --max 0 --stability 1 --dsink 1 --alpha1 0.6 --alpha2 0.2 --time ${timelimit}
                ./bin/R_TheNetworkGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 0 --max 0 --stability 1 --dsink 1 --alpha1 0.45 --alpha2 0.45 --time ${timelimit}
              printf "\\n\\n"
            fi

  done

