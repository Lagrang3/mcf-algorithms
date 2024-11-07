# Fixed Charge Network Flow Problem

The Fixed Charge Network Flow Problem (FCNFP) is defined as a Network Flow
problem for which there is an additional cost for the activation of the arcs, ie. a
flow `x` on arc `a` has a cost `C(x) = c*x + f*Theta(x)`, where `Theta(x)`
is zero if `x` is zero or 1 otherwise, and the values `c` and `f` are
non-negative numbers.

The FCNFP can also be stated as follows
```
given a graph G(N,A)

minimize
    sum_{a in A} x[a] * c[a] + f[a] * y[a]

such that
    0 <= x[a] <= u[a], for all a in A // capacity constraints
    
    y[a] = {0, 1}, for all a in A // y[a] are either 0 or 1
    
    sum_{a outgoing from n} x[a] - sum_{a incoming to n} x[a] = b[n], for all n in N // flow conservation
    
    0 <= x[a] <= y[a] * u[a], for all a in A // y[a] is 1 if and only if x[a]>0

```

This problem is NP-Hard, this can be proven by noticing the decision problem
*Minimum Edge-Cost Flow* (ND32 in [1])
reduces to our current problem with zero proportional costs, `c[a]=0`.

However, we usually don't need to solve the problem exactly and some approximate
solutions exist.

For example Kim-Pardalos [2] describes a *dynamic slope procedure* that produces
approximate solutions by linearizing the problem over a sequence of iterations.

We have implemented Kim-Pardalos's method and tested it with a random set of
testcases. At first with `|N|=30` and `|A|=300` (TODO: investigate how the accuracy of
the solutions evolve with increasing problem size).
We conjeture that the more relevant the fixed costs are the more difficult will
be for the approximate method to find the optimal solution, therefore we
produced a set of random test cases assigning `f[a]` from an uniform PDF in the
range `[0,1000000]` and capacities of the arcs `u[a]` in the range `[0,10000]`
while the proportional costs `c[a]` where extracted from an uniform PDF in
the range `[0,2^k]` with `k` varying in different test cases from 0 to 20.

Results are shown below in a scatter plot.
For each test case there are two points: one corresponding to the dynamic slope
procedure and the other using a one time linearization of the problem.
The X coordinate is the ratio of the mean value of `f[a]` and the mean value of
`c[a]` multiplied by the flow demand in the problem as a measure of how much do
we deviate from a purely linear problem, ie. the smaller the X the more
quasi linear is the problem and the bigger the X the more important are fixed
costs values and the harder the problem gets. In the Y axis we have
the optimal cost as computed by the approximate method divided by the
optimal cost for the exact solution.

![Scatter plot of dynamic slope testing](./dyn-slope-scatter.png)

In general the dynamic slope procedure produces better results than the
*fixed slope* (one time linearization). For quasilinear problems (`X<<1`)
both procedures converge to the optimal solution while for mildly
non-linear case (`0.1<X<1`) the fixed slope tends to deviate from the optimal
solution and eventually produce solutions with costs exceeding twice as much
as the exact solution for highly non-linear problems `X>1`.
While the dynamic slope in most cases is able to find a solution that does not
exceed the 50% cost of the optimal solution even in highly non-linear regimes.

To generate the data I used the following command line instructions
```bash
bash generate-fcnfp.sh | tee dataset | example/ex-fcmcf-approx-validate | tee results | python scatter-plot-fcnfp.py
```

# References

[1] Michael R. Garey and David S. Johnson. Computers and Intractability, a guide
to the Theory of NP-Completeness. ISBN 0-7167-1045-5.

[2] Dukwon Kim, Panos M. Pardalos. A solution approach to the fixed charge
network flow problem using a dynamic slope scaling procedure.
Operations Research Letters 24 (1999) 195-203.
