import networkx as nx
import numpy as np
from ortools.graph.python import min_cost_flow
from ortools.graph.python import max_flow

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

def generate_problem(N_nodes, N_arcs, source, target, M_cap, M_cost):
    G = create_graph(N_nodes, N_arcs, source, target)

    start_nodes = np.array([ i for (i,j) in G.edges() ])
    end_nodes = np.array([ j for (i,j) in G.edges() ])

    capacity = np.random.randint(1, M_cap+1, size=N_arcs)
    cost = np.random.randint(M_cost+1, size=N_arcs)

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
    best_cost = smcf.optimal_cost()
    
    return N_nodes, N_arcs, start_nodes, end_nodes, capacity, cost, amount, best_cost

source = 0
target = 1
N = 5
M = 10
MAXCOST = 10
MAXCAP = 10

N_nodes, N_arcs, start_nodes, end_nodes, capacity, cost, amount, best_cost = generate_problem(N, M, source, target, MAXCAP, MAXCOST)

print(N_nodes, N_arcs)
for i, j, ca, co in zip(start_nodes, end_nodes, capacity, cost):
    print(i, j, ca, co)
print(amount, best_cost)
