#include <cassert>
#include <iostream>

#include <ortools/graph/min_cost_flow.h>

namespace ort = operations_research;

static bool solve_case()
{
       static int c = 0;
       c++;

       int N_nodes, N_arcs;
       std::cin >> N_nodes >> N_arcs;
       if (N_nodes == 0 && N_arcs == 0)
               return false;

       ort::SimpleMinCostFlow mcf;

       for (int i = 0; i < N_arcs; i++) {
               int from, to;
               long long cap, cost;
               std::cin >> from >> to >> cap >> cost;
               mcf.AddArcWithCapacityAndUnitCost(from, to, cap, cost);
       }

       long long amount, best_cost;
       std::cin >> amount >> best_cost;
       mcf.SetNodeSupply(0, amount);  // source
       mcf.SetNodeSupply(1, -amount); // sink

       const auto status = mcf.Solve();
       assert(status == ort::SimpleMinCostFlow::OPTIMAL);

       const auto total_cost = mcf.OptimalCost();
       assert(total_cost == best_cost);

       return true;
}

int main()
{
       /* One test case after another. The last test case has N number of nodes
        * and arcs equal to 0 and must be ignored. */
       while (solve_case())
               ;
       return 0;
}

