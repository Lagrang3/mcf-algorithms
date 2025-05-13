import matplotlib.pyplot as plt
import subprocess
import tempfile
import time
import os

Nodes = [100, 400, 1600, 6400, 25600]
Arcs = [n * 50 for n in Nodes]
Repeat = 10
Cap = 1000000
Cost = 1000000

execs = [
    "./build/example/ex-mcf-validate",
    "./build/example/ex-goldberg-tarjan-validate",
]
execs_label = ["SSP", "Goldberg-Tarjan"]


def gen_test_cases(N_nodes, N_arcs, Max_cap, Max_cost, Repeat):
    fname = tempfile.mktemp(prefix="mcf_bench_{}_".format(N_nodes))
    with open(fname, "a") as out:
        for r in range(Repeat):
            p = subprocess.Popen(
                [
                    "python",
                    "./python/fuzzy.py",
                    "0",
                    "1",
                    str(N_nodes),
                    str(N_arcs),
                    str(Max_cap),
                    str(Max_cost),
                ],
                stdout=out,
            )
            p.wait()
        p = subprocess.Popen(
            ["python", "./python/fuzzy.py", "0", "0", "0", "0", "0", "0"], stdout=out
        )
        p.wait()
    return fname


def run_bench():
    execs_time = dict()
    for l in execs_label:
        execs_time[l] = list()

    for n, a in zip(Nodes, Arcs):
        print(f"Generating cases N_nodes = {n}, N_arcs = {a}")
        testfile = gen_test_cases(n, a, Cap, Cost, Repeat)
        print(f"Temporary file: {testfile}")

        for ex, label in zip(execs, execs_label):
            print(f"Running {ex}")
            start_time = time.time()
            with open(testfile, "r") as f:
                ret = subprocess.run([ex], stdin=f, check=True)
            end_time = time.time()
            T = int(1000 * (end_time - start_time))
            print(f"{ex} finished after {T} msec")
            execs_time[label].append(T / Repeat)

        os.remove(testfile)
    return execs_time


def plot(nodes, execs_time):
    fig, ax = plt.subplots()
    for l in execs_time:
        times = execs_time[l]
        ax.loglog(nodes, times, "-o", label=l)
    ax.set_ylabel("time (ms)")
    ax.set_xlabel("nodes")
    ax.legend()
    plt.show()


execs_time = run_bench()
plot(Nodes, execs_time)
