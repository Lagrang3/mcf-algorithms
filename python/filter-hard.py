# Filters a dataset leaving only those problems that are seemingly hard to solve
# by the approximate method: either because the vanilla solution fails to find a
# feasible solution or because the feasible solution is far from optimal.

import sys

class Problem:
    def __init__(self):
        self._data = {}
        self._arcs = []
        self._constraints = []
        self._proof = []
        
        self['id']='0000000000000000'
        self['num_nodes'] = 0
        self['num_arcs'] = 0
        self['num_constraints'] = 0
        self['difficulty'] = 0
        
        self['source'] = 0
        self['destination'] = 0
        self['flow'] = 0
        self['best_cost'] = 0
    
    def __getitem__(self, key):
        return self._data[key]
    
    def __setitem__(self, key, value):
        self._data[key] = value
    
    @staticmethod
    def read(fd):
        line = fd.readline()
        if not line:
            return None
        p = Problem()
        p['id'] = line.split()[0]
        if p['id']=='0000000000000000':
            return None
        line2 = fd.readline()
        # print("line1", line)
        # print("line2", line2)
        elements = line2.split()
        p['num_nodes'] = int(elements[0])
        p['num_arcs'] = int(elements[1])
        p['num_constraints'] = int(elements[2])
        p['difficulty'] = float(elements[3])
        for i in range(p['num_arcs']):
            line = fd.readline()
            # print("line arc", line)
            p._arcs.append(list(map(int, line.split())))
        line = fd.readline()
        # print("line constraints", line)
        p._constraints = list(map(int, line.split()))
        line = fd.readline()
        # print("line flow", line)
        elements = line.split()
        p['source'] = int(elements[0])
        p['destination'] = int(elements[1])
        p['flow'] = int(elements[2])
        line = fd.readline()
        # print("line cost", line)
        p['best_cost'] = int(line.split()[0])
        line = fd.readline()
        p._proof = list(map(int, line.split()))
        return p
    
    def serialize(self, fd):
        print(self['id'], file=fd)
        print(self['num_nodes'], self['num_arcs'], self['num_constraints'],
            self['difficulty'], file=fd)
        for a in self._arcs:
            print(" ".join(map(str, a)), file=fd)
        print(" ".join(map(str, self._constraints)), file=fd )
        print(self['source'], self['destination'], self['flow'], file=fd)
        print(self['best_cost'], file=fd)
        print(" ".join(map(str, self._proof)), file=fd)

class Result:
    def __init__(self):
        self._data = {}

    @staticmethod
    def read(fd):
        line = fd.readline()
        if not line:
            return None
        r = Result()
        elements = line.split()
        r['id'] = elements[0]
        r['difficulty'] = float(elements[1])
        r['best_cost'] = int(elements[2])
        r['num_constraints'] = int(elements[3])
        r['mcf_cost'] = int(elements[4])
        r['mcf_satisfied'] = int(elements[5])
        r['solution_cost'] = int(elements[6])
        r['solution_satisfied'] = int(elements[7])
        return r
    
    def __getitem__(self, key):
        return self._data[key]
    
    def __setitem__(self, key, value):
        self._data[key] = value
    
    def __str__(self):
        return "a Result: " + str(self._data)


tolerance = 0.5

data_file = sys.argv[1]
result_file = sys.argv[2]

fdata = open(data_file, "r")
fresult = open(result_file, "r")

count_hard = 0
count_all = 0

while True:
    problem = Problem.read(fdata)
    result = Result.read(fresult)
    
    if problem==None or result==None:
        break
    
    assert problem['id']==result['id']
    
    count_all+=1
    
    if result["solution_satisfied"] < problem["num_constraints"] or result["solution_cost"] > (1+tolerance)*problem["best_cost"]:
        problem.serialize(sys.stdout)
        count_hard += 1

Problem().serialize(sys.stdout)
print("count_all", count_all)
print("count_hard", count_hard)
