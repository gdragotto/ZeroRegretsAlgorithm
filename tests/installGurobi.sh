#!/bin/bash
# Gabriele Dragotto - 2022 - ZERO Regrets
# Create some links for the Gurobi libraries
# Gurobi directory candidates
GUROBI_HINTS=("/home/gurobi/10.0.0/linux64" "/Library/gurobi1000/macos_universal2")
echo "Searching for Gurobi in ${GUROBI_HINTS}"

echo "If you want want to specify another directory, please edit this bash."

GUROBILIB=""
GUROBILIBCXX=""


for DIRECTORY in ${GUROBI_HINTS[@]}; do

  if [ -d "$DIRECTORY" ]; then
    GUROBI_VERSION=$(echo "$DIRECTORY" | grep -o  '[0-9].[0-9]')
    if [ -z "$GUROBI_VERSION" ]; then
      GUROBI_VERSION=$(echo "$DIRECTORY" | grep -o  '[0-9][0-9]')
    fi
    GUROBI_VERSION=${GUROBI_VERSION/./}
    GUROBI_VERSION=${GUROBI_VERSION:0:2}
    echo -e "\tVersion of ${DIRECTORY} is ${GUROBI_VERSION}"


    GUROBILIBCXX=$(find $DIRECTORY/lib/libgurobi_g++5.2.* 2> /dev/null)
    if [[ -f $GUROBILIBCXX ]]; then
        echo -e "\tGUROBILIBCXX found (g++5.2)"
    else
       GUROBILIBCXX=$(find $DIRECTORY/lib/libgurobi_c++.* 2> /dev/null)
      if [[ -f $GUROBILIBCXX ]]; then
        echo -e "\tGUROBILIBCXX found (c++)"
      else
        echo -e "\tGUROBILIBCXX not found!"
        exit
      fi
    fi

    GUROBILIB=$(find $DIRECTORY/lib/libgurobi$GUROBI_VERSION.* 2> /dev/null)
    if [[ -f $GUROBILIB ]]; then
        echo -e "\tGUROBILIB found"
    else
      echo -e "\tGUROBILIB not found!"
      exit
    fi

    rm -rf ./gurobi
    mkdir gurobi
    GUROBI_FILE_EXTENSION="${GUROBILIB##*.}"
    ln -s ${GUROBILIB} ./gurobi/libgurobi.${GUROBI_FILE_EXTENSION}
    ln -s ${GUROBILIBCXX} ./gurobi/libgurobi_c++.${GUROBI_FILE_EXTENSION}
  fi
done

