from sys import stdin
import math
import numpy as np
import matplotlib.pyplot as plt

n_constraints = 0
fixed_charge = False

def transform_scale(x):
    return -np.log2(1.0 - x)

def mkboxes(X, Y):
    pos = list(set([i for i in X]))
    pos.sort()
    idx = {}
    for i, x in enumerate(pos):
        idx[x] = i
    boxes = [ [] for i in range(len(pos))]
    for x,y in zip(X,Y):
        boxes[idx[x]].append(y)
    return boxes, pos

def plot_scatter(X, Y):
    quantiles = [0.5, 0.75, 0.95, 0.99]

    X_transf = transform_scale(X)
    boxes, positions = mkboxes(X_transf, Y)
    X_ticks = [i for i in range(2+int(max(X_transf)))]
    X_labels = ["%.2f" % (1. - 1./2**i) for i in X_ticks]
    
    fig, ax = plt.subplots()
    ax.set_ylim(0.999, 1.6)
    # ax.boxplot(boxes, positions=positions)
    ax.violinplot(boxes, positions=positions)
    ax.set_xticks(X_ticks)
    ax.set_xticklabels(X_labels)
    ax.set_xlabel("difficulty")
    ax.set_ylabel("approximate/best solution ratio")
    ax.set_title("%d-constraints solution accuracy (%s activation costs)" % (n_constraints-1,
        "with" if fixed_charge else "without"))
    
    qvalues = np.array([ np.quantile(b, quantiles) for b in boxes ])
    qvalues = qvalues.transpose()
    
    for i in range(len(quantiles)):
        ax.plot(positions, qvalues[i], label = "%.2f%%" % (100*quantiles[i]))
    
    plt.legend()
    plt.show()


def plot_successrate(X, Y):
    X_transf = transform_scale(X)
    boxes, positions = mkboxes(X_transf, Y)
    
    positions = [float(p) for p in positions ]
    Y_rate = [ float(sum(b)/len(b)) for b in boxes ]
    
    X_ticks = [i for i in range(2+int(max(X_transf)))]
    X_labels = ["%.2f" % (1. - 1./2**i) for i in X_ticks]
    
    fig, ax = plt.subplots()
    ax.plot(positions, Y_rate, ls='', marker='o')
    
    ax.set_xticks(X_ticks)
    ax.set_xticklabels(X_labels)
    ax.set_xlabel("difficulty")
    ax.set_ylabel("success rate")
    ax.set_title("%d-constraints success rate (%s activation costs)" % (n_constraints-1, 
        "with" if fixed_charge else "without"))
    plt.show()

# all results
DIFF = []
SUCCESS = []

# only those results where constraints are satisfied
DIFF_filtered = []
COST_relative = []

for line in stdin:
    elements = line.split()
    pid = elements[0]
    d = float(elements[1])
    exact, nc, mcf, nc_mcf, approx, nc_approx = map(int, elements[2:])
    
    n_constraints = nc
    
    DIFF.append(d)
    # constraints satisfied
    assert nc_approx<=nc
    SUCCESS.append(int(nc_approx==nc))
    
    if nc_approx==nc:
        DIFF_filtered.append(d)
        # the approximate is never better than the exact
        assert exact<=approx
        COST_relative.append(approx/exact)
        
        if COST_relative[-1]>1.5:
            print(line)

plot_scatter(np.array(DIFF_filtered), np.array(COST_relative))
plot_successrate(np.array(DIFF), np.array(SUCCESS))
