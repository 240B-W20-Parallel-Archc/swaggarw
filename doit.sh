#!/bin/bash
make clean
make

#Random Read Access
  for i in 1 2 4 8 16
  do
        echo -e "Random Read Access:\n"
        python3 cachetime.py -r -t ${i}
  done

 #Random RMW Access
 for i in 1 2 4 8 16
 do
        echo -e "Random RMW Access:\n"
        python3 cachetime.py -r -m -t ${i}
 done