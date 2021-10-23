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
        for it in np.arange(10):
            cmd = "../build/graph_cover -n " + str(int(n)) + " " + str(p) + " >> graphs_compare_couplage_glouton/data.txt"
            os.system(cmd)

def create_graphs(Nmax):
    data = open("graphs_compare_couplage_glouton/data.txt", "r")
    lines = data.readlines()
    
    step = Nmax / 10
    x = [int(n) for n in np.arange(step, Nmax+step, step)]
    y_tc = []
    y_tg = []
    y_sc = []
    y_sg = []
    # si count == 40 alors on a une donnée (x,y)
    count = 0
    mod = 0
    
    t_couplage = 0
    t_glouton = 0
    s_couplage = 0
    s_glouton = 0

    for line in lines:
        l = line[0:len(line)-1]
        value = int(line)

        if len(line) == 0:
            continue
        if mod == 0:
            t_couplage += value
        elif mod == 1:
            t_glouton += value
        elif mod == 2:
            s_couplage += value
        elif mod == 3:
            s_glouton += value

        mod += 1
        if mod == 4:
            mod = 0
        count += 1
        if count == 40:
            y_tc.append(t_couplage/10.0)
            y_tg.append(t_glouton/10.0)
            y_sc.append(int(s_couplage/10))
            y_sg.append(int(s_glouton/10))
            t_couplage = 0
            t_glouton = 0
            s_couplage = 0
            s_glouton = 0
            count = 0

    # graphs
    max_tc = max(y_tc)
    max_tg = max(y_tg)
    max_t = max(max_tc, max_tg)
    max_sc = max(y_sc)
    max_sg = max(y_sg)
    max_s = max(max_sc, max_sg)
    y_tc = [i if i == 0 else np.log(i) for i in y_tc]
    y_tg = [i if i == 0 else np.log(i) for i in y_tg]

    # création du graphe des temps
    plt.figure(figsize=(15,15))
    plt.grid(True, "both")
    plt.minorticks_on()
    plt.plot(x,y_tc, label = "algo couplage", c="blue")
    plt.plot(x,y_tg, label = "algo glouton", c="red")
    plt.legend(fontsize=15)
    plt.title("Temps d'éxecution des algos couplage et glouton en fonction de n\n(échelle log)")
    plt.xlabel("nombre de sommets n")
    plt.ylabel("temps d'éxecution (microseconde)")
    plt.savefig("graphs_compare_couplage_glouton/temps_couplage_glouton.png")
    plt.show()

    # création du graphe des solutions
    plt.grid(True, "both")
    plt.minorticks_on()
    plt.plot(x,y_sc, label = "algo couplage", c="blue")
    plt.plot(x,y_sg, label = "algo glouton", c="red")
    plt.legend(fontsize=15)
    plt.title("Taille de couverture des algos couplage et glouton en fonction de n")
    plt.xlabel("nombre de sommets n")
    plt.ylabel("taille de la couverture")
    plt.savefig("graphs_compare_couplage_glouton/solutions_couplage_glouton.png")

argLen = len(sys.argv)
n = 0 # number of vertex
p = 0.0 # probability of connectivity

if argLen == 3:
    n = int(sys.argv[1])
    p = Decimal(sys.argv[2])
    do_tests(n, p)
    create_graphs(n)
else:
    print("Erreur : nombre d'arguments incorrect.\n")
    print("Usage : ./cmp_couplage_glouton.py <vertex_count_MAX> <probability>")
    sys.exit(-1)
