import networkx as nx
import numpy as np
from ortools.graph.python import min_cost_flow
from ortools.graph.python import max_flow
from ortools.linear_solver import pywraplp

def create_graph(N_nodes, N_arcs, source, target):
    G = None
    while True: 
        G = nx.gnm_random_graph(N_nodes, N_arcs, directed=True)
        try:
            path = nx.shortest_path(G, source=source, target=target)
        except:
            continue
        break
    return G

def linear_solver(start_nodes, end_nodes, capacity, cost, fixedcost, supply):
    '''
    Use ortools to solve this linear problem
    '''
    solver = pywraplp.Solver.CreateSolver('SAT')
    
    N_arcs = len(start_nodes)
    N_nodes = len(supply)
    
    xvar = [ solver.IntVar(lb=0, ub=0.1+capacity[i], name=f'x_{i}')  for i in range(N_arcs) ]
    yvar = [ solver.IntVar(lb=0, ub=1.1, name=f'y_{i}')  for i in range(N_arcs) ]
    
    for n in range(N_nodes):
        outgoing=0
        incoming=0
        for e in range(N_arcs):
            if start_nodes[e]==n:
                outgoing += xvar[e]
            if end_nodes[e]==n:
                incoming += xvar[e]
        solver.Add(supply[n]==outgoing-incoming)
    
    for i in range(N_arcs):
        solver.Add(xvar[i]<=yvar[i]*capacity[i])
    
    solver.Minimize(sum([ xvar[i]*cost[i]+yvar[i]*fixedcost[i] for i in range(N_arcs)]))
    
    status = solver.Solve()
    return int(solver.Objective().Value())

def generate_problem(N_nodes, N_arcs, source, target, M_cap, M_cost, M_charge):
    G = create_graph(N_nodes, N_arcs, source, target)

    start_nodes = np.array([ i for (i,j) in G.edges() ])
    end_nodes = np.array([ j for (i,j) in G.edges() ])
    N_arcs = len(G.edges())

    capacity = np.random.randint(1, M_cap+1, size=N_arcs)
    cost = np.random.randint(M_cost+1, size=N_arcs)
    fixedcost = np.random.randint(M_charge+1, size=N_arcs)

    # find out the max flow
    smf = max_flow.SimpleMaxFlow()
    all_arcs = smf.add_arcs_with_capacity(start_nodes, end_nodes, capacity)
    mf_status = smf.solve(source, target)
    assert mf_status == smf.OPTIMAL
    maxf = smf.optimal_flow()
    
    # send amount [1, Max.Flow]
    amount = np.random.randint(maxf) + 1
    smcf = min_cost_flow.SimpleMinCostFlow()
    supply = [0]*N_nodes
    supply[source] = amount
    supply[target] = -amount
    all_arcs = smcf.add_arcs_with_capacity_and_unit_cost(
        start_nodes, end_nodes, capacity, cost
    )
    smcf.set_nodes_supplies(np.arange(0, N_nodes), supply)
    status = smcf.solve()
    assert status == smcf.OPTIMAL
    best_mcfcost = smcf.optimal_cost()
    
    best_fcmcfcost = linear_solver(start_nodes, end_nodes, capacity, cost, fixedcost, supply)
    
    return N_nodes, N_arcs, start_nodes, end_nodes, capacity, cost, fixedcost, amount, best_fcmcfcost

import sys

source, target, N, M, MAXCOST, MAXCHARGE, MAXCAP = map(int, sys.argv[1:])

if N==0:
    print("0 0")
    exit()

N_nodes, N_arcs, start_nodes, end_nodes, capacity, cost, fixedcost, amount, best_cost = generate_problem(N, M, source, target, MAXCAP, MAXCOST, MAXCHARGE)

print(N_nodes, N_arcs)
for i, j, ca, co, fco in zip(start_nodes, end_nodes, capacity, cost, fixedcost):
    print(i, j, ca, co, fco)
print(amount, best_cost)
