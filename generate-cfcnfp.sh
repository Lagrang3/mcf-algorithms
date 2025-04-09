#!/bin/bash

if [ $# -ne 2 ]; then
        echo "Usage: $0 N_CONSTRAINTS HAVE_ACTIVATION_COSTS"
        exit 1
fi

# Generate a Constrained Fixed Charge Network Flow Problem set

# number of node
TOP_N=30

# number of arcs
TOP_M=100

# maximum capacity
MAX_CAP=10000

# maximum cost
MAX_COST=100

# to make activation cost competitive with linear cost we need
# <MAX_CAP><MAX_COST> ~ <MAX_CHARGE>

# maximum fixed charge
if [ "$2" == "Y" ]; then
        MAX_CHARGE=100000
else
        MAX_CHARGE=0
fi

# number of constraints
# not much sense in this, because we already want to optimize this quantity and
# we have let the constrait for this quantity to be very loose 0.1

N_CONSTRAINTS=$1

COPIES=1000

repeat(){
	for i in `seq 2 ${N_CONSTRAINTS}`;do
		echo -n "$1 ";	
	done
}

for i in `seq ${COPIES}`;
do
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.01`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.25`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.5`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.6`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.7`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.8`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.85`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.90`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.95`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.96`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.97`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.98`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.985`
	python ./python/fuzzy-constrained-fcnfp.py ${TOP_N} ${TOP_M} ${MAX_CAP} ${MAX_COST} ${MAX_CHARGE} ${N_CONSTRAINTS} 0.01 `repeat 0.990`
done

echo "0000000000000000"
echo "0 0 0"
