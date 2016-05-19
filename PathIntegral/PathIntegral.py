#coding: utf-8
import random
import math as m
N = 100

def Energy(path):
    ans = 0
    for i in range(N-1):
        ans += (path[i+1] - path[i])**2# + path[i]*path[i*1]
    return ans

#path = [random.randint(-9, 9)*0.1 for i in range(N)]
path = [0 for i in range(N)]
psi = [0.0 for i in range(N)]

# Current Energy
old_E = Energy(path)
rep = 10000
for i in range(rep):
    # Where do you change?
    site = random.randint(0,N-1)

    # How much do you change?
    change = random.randint(-9, 9)*0.1

    # Flip
    path[site] += change
    new_E = Energy(path)

    # Reject
    if(new_E > old_E and m.exp(-new_E + old_E) <= random.random()):
        path[site] -= change

    """
    for j in range(N):
        site = int(path[j]*10) + int(N/2)
        psi[site] += 1
    """
    site = int(path[site]*10) + int(N/2) -1
    psi[site] += 1
    old_E = new_E

for index, elem in enumerate(psi):
    print str(index) + "\t" + str(elem/rep)
