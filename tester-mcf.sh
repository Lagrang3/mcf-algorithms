#!/bin/bash

fname=testcases.data

echo -n "" > $fname

python ./python/fuzzy.py 0 1 2 1 10 10 >> $fname
python ./python/fuzzy.py 0 1 2 2 10 10 >> $fname
python ./python/fuzzy.py 0 1 2 3 10 10 >> $fname
python ./python/fuzzy.py 0 1 3 5 10 10 >> $fname
python ./python/fuzzy.py 0 1 3 5 10 10 >> $fname
python ./python/fuzzy.py 0 1 5 5 10 10 >> $fname
python ./python/fuzzy.py 0 1 5 6 10 10 >> $fname
python ./python/fuzzy.py 0 1 5 7 10 10 >> $fname
python ./python/fuzzy.py 0 1 5 8 10 10 >> $fname
python ./python/fuzzy.py 0 1 5 9 10 10 >> $fname
python ./python/fuzzy.py 0 1 5 10 10 10 >> $fname
python ./python/fuzzy.py 0 1 5 11 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 10 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 11 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 12 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 13 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 14 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 15 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 16 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 17 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 18 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 19 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 20 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 50 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 60 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 70 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 80 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 90 10 10 >> $fname
python ./python/fuzzy.py 0 1 10 90 10 10 >> $fname
python ./python/fuzzy.py 0 1 100 5000 10 10 >> $fname
python ./python/fuzzy.py 0 1 100 5000 10 10 >> $fname
python ./python/fuzzy.py 0 1 100 5000 10 10 >> $fname
python ./python/fuzzy.py 0 1 100 5000 10 10 >> $fname
python ./python/fuzzy.py 0 1 100 5000 10 10 >> $fname
python ./python/fuzzy.py 0 1 1000 50000 100 100 >> $fname
python ./python/fuzzy.py 0 1 1000 50000 100 100 >> $fname
python ./python/fuzzy.py 0 1 1000 50000 100 100 >> $fname
python ./python/fuzzy.py 0 1 1000 50000 100 100 >> $fname
python ./python/fuzzy.py 0 1 1000 50000 100 100 >> $fname
python ./python/fuzzy.py 0 0 0 0 0 0 >> $fname

./build/example/ex-mcf-validate < $fname
./build/example/ex-goldberg-tarjan-validate < $fname
