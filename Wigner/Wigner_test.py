# coding: utf-8

import numpy as np
#import scipy as sp
from scipy.integrate import odeint
import matplotlib.pyplot as plt
import math as m


"""
diffusion equation
∂f(x,t)/∂t = ∂^2f(x,t)/∂x^2
↓
n+1 simultaneous differential equation for f0 ~ fn
where fk = f(k*dx,t), dx = L/N

df0/dt = D/dx^2*(f1 - 2*f0)
df1/dt = D/dx^2*(f2 - 2f1 + f0)
...
dfk/dt = D/dx^2*(f(k+1) - 2fk + f(k-1))
...
df(n-1)/dt = D/dx^2*(fn - 2f(n-1) + f(n-2))
dfn/dt = d/dx^2*(-2*fn + f(n-1))

"""


# set differential equation
def equation(f, t=0, D=1.0, N=200, L=50.0):
    dx = L/N
    gamma = D/dx/dx
    arr = []

    """ f0 """
    arr.append(gamma*(f[1]-2*f[0]))
    """ f1 ~ fN-1 """
    arr += [gamma*(f[i+1]-2*f[i]+f[i-1]) for i in range(1, N)]
    """ fN """
    arr.append(gamma*(-2*f[N]+f[N-1]))

    return np.array(arr)

# set initial function
gaussian = lambda x, D=1: (2*m.pi)**(-0.5)*m.exp(-0.5/D*x**2)


def main():
    # initial parameter(optional)
    D, N, L = 1.0, 200, 40.0
    args = (D, N, L)

    # coordinate
    x_div = N+1
    q = np.linspace(-L/2, L/2, x_div)

    # initial value for each fk
    fk_0 = np.array([gaussian(x, D) for x in q])

    # time
    t_max, t_div = 20.0, 10
    t = np.linspace(0, t_max, t_div)

    # solve
    trajectories = odeint(equation, fk_0, t, args, Dfun=None)
    # trajectories = odeint(f, y0, t, Dfun=None)
    # print(trajectories)

    # plot
    for i in range(t_div):
        l = "t = " + str(t_max*float(i)/t_div)
        plt.plot(q, trajectories[i], label=l)

    plt.xlim(-L/2, L/2)
    plt.ylim(0, 0.4)
    plt.xlabel("x")
    plt.ylabel("f(x,t)")
    plt.title("Solution of diffusion equation")
    plt.legend()
    plt.show()

if __name__ == "__main__":
    main()
