#!/bin/bash
  instanceFolder=../instance_generators/instances_kp/generated
  resultsFolder=../dat/
  logFile=${resultsFolder}/resultsKP.csv
  numThreads=8
  timelimit=1800
  mv bin/TheKnapsackGame bin/R_TheKnapsackGame
  for n in 2 3
  do
      for m in 25 50 75 100
      do
        for ins in 2 5 8
        do
            instanceFile=${instanceFolder}/${n}-${m}-${ins}-pot.txt
            if test -f "$instanceFile"; then
              instanceName=${instanceFile/.txt/}
              printf "####################### ${instanceName} #######################\n"
                ./bin/R_TheKnapsackGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 0 --stability 1  --max 1 --time ${timelimit}
              printf "\\n\\n"
            fi

            instanceFile=${instanceFolder}/${n}-${m}-${ins}-cij.txt
            if test -f "$instanceFile"; then
              instanceName=${instanceFile/.txt/}
              printf "####################### ${instanceName} #######################\n"
                ./bin/R_TheKnapsackGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 0 --stability 1  --max 1 --time ${timelimit}
              printf "\\n\\n"
            fi

            instanceFile=${instanceFolder}/${n}-${m}-${ins}-cij-n.txt
            if test -f "$instanceFile"; then
              instanceName=${instanceFile/.txt/}
              printf "####################### ${instanceName} #######################\n"
                ./bin/R_TheKnapsackGame -f ${instanceFile} -t ${numThreads} --log ${logFile} --enumerate 0 --stability 1  --max 1 --time ${timelimit}
              printf "\\n\\n"
            fi

        done
      done
  done

