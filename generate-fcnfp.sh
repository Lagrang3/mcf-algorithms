#!/bin/bash

TOP_N=30
TOP_M=300
COPIES=100

for i in `seq ${COPIES}`;
do
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 1 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 2 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 4 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 8 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 16 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 32 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 64 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 128 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 256 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 512 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 1024 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 2048 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 4096 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 8192 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 16384 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 32768 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 65536 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 131072 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 262144 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 524288 1000000 10000
	python ./python/fuzzy-fcnfp.py 0 1 ${TOP_N} ${TOP_M} 1048576 1000000 10000
done

echo "0 0"
