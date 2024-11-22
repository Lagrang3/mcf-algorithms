from sys import stdin
import math
import numpy as np
import matplotlib.pyplot as plt

def filter_range(X, Y, a, b):
    X_new = []
    Y_new = []
    for x,y in zip(X,Y):
        if x<a or x>=b:
            continue
        X_new.append(x)
        Y_new.append(y)
    return X_new, Y_new

def plot_scatter(X, Y_1, Y_2):
    fig, ax = plt.subplots()

    ax.loglog(X, Y_1, marker='+', ls='', label = "dynamic slope")
    ax.loglog(X*1.1, Y_2, marker='x', ls='', label = "fixed slope")
    ax.set_ylabel("approximate solution/best solution")
    ax.set_xlabel("<fixed cost>/(<proportional cost> * flow)")

    xmin=min(X)
    xmax=max(X)
    ax.hlines(1, xmin=xmin,xmax=xmax, linestyles='--',
        colors='black', alpha=0.5)
    ax.text(xmax,1,'Best', verticalalignment='bottom')
    ax.hlines(1.1, xmin=xmin, xmax=xmax, linestyles='--',
        colors='black', alpha=0.5)
    ax.text(xmax,1.1,'+10%', verticalalignment='bottom')
    ax.hlines(1.2, xmin=xmin, xmax=xmax, linestyles='--',
        colors='black', alpha=0.5)
    ax.text(xmax,1.2,'+20%', verticalalignment='bottom')
    ax.hlines(1.5, xmin=xmin, xmax=xmax, linestyles='--',
        colors='black', alpha=0.5)
    ax.text(xmax,1.5,'+50%', verticalalignment='bottom')

    yticks = [1,2,3,4,5,6,7,8]
    ax.set_yticks(yticks)
    ax.set_yticklabels(yticks)
    ax.set_ylim(bottom=0.85)
    ax.legend()
    plt.show()

def plot_histogram(Y):
    fig, ax = plt.subplots()
    ax.hist(Y, bins=[1+0.1*i for i in range(10)])
    plt.show()

Y_mcf = []
Y_ds = []
X = []

for line in stdin:
    ybest,ymcf,yds,mean_f,mean_c,flow = map(float, line.split())
    Y_mcf.append(ymcf/ybest)
    Y_ds.append(yds/ybest)
    X.append(mean_f/(flow*mean_c))

Y_ds = np.array(Y_ds)
Y_mcf = np.array(Y_mcf)
X = np.array(X)

plot_scatter(X, Y_ds, Y_mcf)

X_1, Y_1 = filter_range(X, Y_ds, 0.0001, 0.001)
plot_histogram(Y_1)
X_1, Y_1 = filter_range(X, Y_ds, 0.001, 0.01)
plot_histogram(Y_1)
X_1, Y_1 = filter_range(X, Y_ds, 0.01, 0.1)
plot_histogram(Y_1)
X_1, Y_1 = filter_range(X, Y_ds, 0.1, 1)
plot_histogram(Y_1)
X_1, Y_1 = filter_range(X, Y_ds, 1, 10)
plot_histogram(Y_1)
