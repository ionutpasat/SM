import numpy as np
import matplotlib.pyplot as plt

def lab2():
    x = np.arange(1, 32.1, 0.1)
    
    fig, ax = plt.subplots()
    ax2 = ax.twinx()
    
    ax.plot(x, En_output(x), 'r', label='En')
    ax.plot(x, Un_output(x), 'b', label='Un')
    ax2.plot(x, Sn_output(x), 'g', label='Sn')
    ax2.plot(x, Rn_output(x), 'c', label='Rn')
    ax2.plot(x, Qn_output(x), 'm', label='Qn')
    
    ax.set_xlabel('n')
    ax.set_ylabel('E(n) U(n)', color='black')
    ax2.set_ylabel('S(n), R(n), Q(n)', color='black')
    
    ax.legend(loc='upper left')
    ax2.legend(loc='upper right')
    
    plt.title('Exercitiul 1')
    plt.show()

def O1_input(n):
    return n**3

def T1_input(n):
    return n**3

def On_input(n):
    return n**3 + n**2 * np.log2(n)

def Tn_input(n):
    return 4 * n**3 / (n + 3)

def Sn_output(n):
    return T1_input(n) / Tn_input(n)

def En_output(n):
    return T1_input(n) / (n * Tn_input(n))

def Rn_output(n):
    return On_input(n) / O1_input(n)

def Un_output(n):
    return On_input(n) / (n * Tn_input(n))

def Qn_output(n):
    return T1_input(n)**3 / (n * Tn_input(n)**2 * On_input(n))

lab2()