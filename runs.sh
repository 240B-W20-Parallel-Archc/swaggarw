#/bin/bash

make clean
make

for i in 1 2 4 8 16
do
 python3 cachetime.py -r -t ${i} cachetime.out
done

for i in 1 2 4 8 16
do
 python3 cachetime.py -r -m -t ${i} cachetime.out
done

for i in 1 2 4 8 16
do
  python3 cachetime.py -s -ss 64 -t ${i} cachetime.out
done

for i in 1 2 4 8 16
do
  python3 cachetime.py -m -s -ss 64 -t ${i} cachetime.out
done
