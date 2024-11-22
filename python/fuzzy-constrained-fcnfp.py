import networkx as nx
import numpy as np
from ortools.graph.python import min_cost_flow
from ortools.graph.python import max_flow
from ortools.linear_solver import pywraplp
import matplotlib.pyplot as plt

def create_graph(N_nodes, N_arcs, source, target):
    """
    Create a random graph where the source and target are connected.
    """
    G = None
    while True:
        G = nx.gnm_random_graph(N_nodes, N_arcs, directed=True)
        try:
            path = nx.shortest_path(G, source=source, target=target)
        except:
            continue
        break
    return G


def solve_fcnfp(start_nodes, end_nodes, capacity, cost, fixedcost, supply):
    """
    Use ortools to solve a Fixed Charge Network Flow Problem
    """
    solver = pywraplp.Solver.CreateSolver("SAT")

    N_arcs = len(start_nodes)
    N_nodes = len(supply)

    # variables domain and capacity constraints
    xvar = [
        solver.IntVar(lb=0, ub=0.1 + capacity[i], name=f"x_{i}") for i in range(N_arcs)
    ]
    yvar = [solver.IntVar(lb=0, ub=1.1, name=f"y_{i}") for i in range(N_arcs)]

    # flow conservation, supply/demand
    for n in range(N_nodes):
        outgoing = 0
        incoming = 0
        for e in range(N_arcs):
            if start_nodes[e] == n:
                outgoing += xvar[e]
            if end_nodes[e] == n:
                incoming += xvar[e]
        solver.Add(supply[n] == outgoing - incoming)

    # x and y variables coupling
    for i in range(N_arcs):
        solver.Add(xvar[i] <= yvar[i] * capacity[i])

    # objective function
    solver.Minimize(
        sum([xvar[i] * cost[i] + yvar[i] * fixedcost[i] for i in range(N_arcs)])
    )

    status = solver.Solve()
    if status == pywraplp.Solver.OPTIMAL:
        return int(solver.Objective().Value())
    # not feasible
    return None

def solve_constrained_fcnfp(
    start_nodes, end_nodes, capacity, cost, fixedcost, supply, constraints
):
    """
    Use ortools to solve a Constrained Fixed Charge Network Flow Problem
    """
    solver = pywraplp.Solver.CreateSolver("SAT")

    N_arcs = len(start_nodes)
    N_nodes = len(supply)

    # variables domain and capacity constraints
    xvar = [
        solver.IntVar(lb=0, ub=0.1 + capacity[i], name=f"x_{i}") for i in range(N_arcs)
    ]
    yvar = [solver.IntVar(lb=0, ub=1.1, name=f"y_{i}") for i in range(N_arcs)]

    # flow conservation, supply/demand
    for n in range(N_nodes):
        outgoing = 0
        incoming = 0
        for e in range(N_arcs):
            if start_nodes[e] == n:
                outgoing += xvar[e]
            if end_nodes[e] == n:
                incoming += xvar[e]
        solver.Add(supply[n] == outgoing - incoming)

    # x and y variables coupling
    for i in range(N_arcs):
        solver.Add(xvar[i] <= yvar[i] * capacity[i])

    # side constraints
    for k in range(len(constraints)):
        solver.Add(
            sum(
                [
                    xvar[i] * cost[k][i] + yvar[i] * fixedcost[k][i]
                    for i in range(N_arcs)
                ]
            )
            <= constraints[k]
        )

    # objective function
    solver.Minimize(
        sum([xvar[i] * cost[0][i] + yvar[i] * fixedcost[0][i] for i in range(N_arcs)])
    )

    status = solver.Solve()
    if status == pywraplp.Solver.OPTIMAL:
        return int(solver.Objective().Value()), [ int(x.solution_value()) for x in xvar ]
    # not feasible
    return None, None


def generate_problem(
    N_nodes, N_arcs, M_cap, M_cost, M_charge, N_constraints, difficulty
):
    """
    Generate a constrained FCNFP, the solution is always feasible.
    We use ortools to compute to explore exact solutions.
    """
    assert len(difficulty)==N_constraints

    assert N_nodes>=2
    source = np.random.randint(N_nodes)
    while True:
        target = np.random.randint(N_nodes)
        if target != source:
            break
    
    G = create_graph(N_nodes, N_arcs, source, target)

    start_nodes = np.array([i for (i, j) in G.edges()])
    end_nodes = np.array([j for (i, j) in G.edges()])
    N_arcs = len(G.edges())
    
    # attempt to build a feasible solution
    attempts = 0
    while True:
        capacity = np.random.randint(1, M_cap + 1, size=N_arcs)
        cost = np.random.randint(M_cost + 1, size=(N_constraints, N_arcs))
        fixedcost = np.random.randint(M_charge + 1, size=(N_constraints, N_arcs))

        # find out the max flow
        smf = max_flow.SimpleMaxFlow()
        all_arcs = smf.add_arcs_with_capacity(start_nodes, end_nodes, capacity)
        mf_status = smf.solve(source, target)
        assert mf_status == smf.OPTIMAL
        maxf = smf.optimal_flow()

        # send amount [1, Max.Flow]
        amount = np.random.randint(maxf) + 1
        supply = [0] * N_nodes
        supply[source] = amount
        supply[target] = -amount

        def difficulty_bound(bound, difficulty):
            """
            A difficult bound
            """
            assert difficulty > 0 and difficulty <= 1
            return bound / difficulty

        # what is the minim value we can obtain for each constraint
        constraints = np.zeros(N_constraints, dtype=int)
        for k in range(len(constraints)):
            lower_bound = solve_fcnfp(
                start_nodes, end_nodes, capacity, cost[k], fixedcost[k], supply
            )
            assert lower_bound is not None
            constraints[k] = difficulty_bound(lower_bound, difficulty[k])

        best_fcmcfcost, xsolution = solve_constrained_fcnfp(
            start_nodes, end_nodes, capacity, cost, fixedcost, supply, constraints
        )
        if best_fcmcfcost is not None:
            break
        
        attempts += 1
        if attempts > 100:
            # too many attempts
            return None

    problem = {}
    problem["graph"] = G
    problem["N_nodes"] = N_nodes
    problem["N_arcs"] = N_arcs
    problem["N_constraints"] = N_constraints
    problem["start_nodes"] = start_nodes
    problem["end_nodes"] = end_nodes
    problem["capacity"] = capacity
    problem["cost"] = cost
    problem["fixedcost"] = fixedcost
    problem["constraints"] = constraints
    problem["amount"] = amount
    problem["source"] = source
    problem["destination"] = target
    problem["solution"] = best_fcmcfcost
    problem["proof"] = xsolution
    return problem


import sys

N, M, MAXCAP, MAXCOST, MAXCHARGE, NCONSTRAINTS = map(int, sys.argv[1:7])
difficulty = [ float(x) for x in sys.argv[-NCONSTRAINTS:]]

if N == 0:
    print("0 0 0")
    exit()

problem = generate_problem(
    N, M, MAXCAP, MAXCOST, MAXCHARGE, NCONSTRAINTS, difficulty
)
if problem is None:
    # too difficult to build a random feasible problem
    exit()

#print(problem)

print(problem["N_nodes"], problem["N_arcs"], problem["N_constraints"])
for i, j, ca, co, fco in zip(
    problem["start_nodes"],
    problem["end_nodes"],
    problem["capacity"],
    problem["cost"].transpose(),
    problem["fixedcost"].transpose(),
):
    print(i, j, ca, end=" ")
    for a, b in zip(co, fco):
        print(a, b, end=" ")
    print()
for k, val in enumerate(problem["constraints"]):
    print(val, end=' ')
print()
print(problem["source"], problem["destination"], problem["amount"])
print(problem["solution"])
for x in problem["proof"]:
    print(x, end=" ")
print()

#nx.draw(problem["graph"], with_labels=True)
#plt.show()
