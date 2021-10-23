#! /usr/bin/python3
# -*- coding: utf-8 -*-

import os
import sys
import time
import matplotlib.pyplot as plt
import numpy as np
from decimal import Decimal

def do_tests(Nmax, p):
    if os.path.isfile("graphs_compare_couplage_glouton/data.txt"):
        os.remove("graphs_compare_couplage_glouton/data.txt")
    step = Nmax / 10
    for n in np.arange(step, Nmax+step, step):
        cmd = "../build/graph_cover -n " + str(int(n)) + " " + str(p) + " >> graphs_compare_couplage_glouton/data.txt"
        os.system(cmd)

def create_graphs(Nmax, p):
    data = open("graphs_compare_couplage_glouton/data.txt", "r")
    lines = data.readlines()
    
    step = Nmax / 10
    x = [int(n) for n in np.arange(step, Nmax+step, step)]
    y = []

    for line in lines:
        l = line[0:len(line)-1]
        value = int(line)

        if len(line) == 0:
            continue

        y.append(value)

    # graphs
    y = [i if i == 0 else np.log(i) for i in y]

    # création du graphe des temps
    plt.figure(figsize=(15,15))
    plt.grid(True, "both")
    plt.minorticks_on()
    plt.plot(x,y, label = "branch & bound v3", c="blue")
    plt.legend(fontsize=15)
    plt.title("Temps d'éxecution en fonction de n\nAlgo branchement avec bornes amélioré et sélection sommet degré max\néchelle log (p = 0.5)")
    plt.xlabel("nombre de sommets n")
    plt.ylabel("temps d'éxecution (microseconde)")
    plt.savefig("graphs_compare_couplage_glouton/temps_branch_bound_v3.png")

argLen = len(sys.argv)
n = 0 # number of vertex
p = 0.0 # probability of connectivity

if argLen == 3:
    n = int(sys.argv[1])
    p = Decimal(sys.argv[2])
    do_tests(n, p)
    create_graphs(n, p)
else:
    print("Erreur : nombre d'arguments incorrect.\n")
    print("Usage : ./test_branch.py <vertex_count_MAX> <probability>")
    sys.exit(-1)
