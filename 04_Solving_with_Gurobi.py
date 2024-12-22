import sys

from numpy import *
import gurobipy
import os

def rdnumpy(txtname):
    f = open(txtname)
    line = f.readlines()
    lines = len(line)  # number of rows
    cnt=0
    for l in line:
        le = l.strip('\n').split(' ')

        columns = len(le)-1  # number of columns

    A = zeros((lines, columns), dtype=int)
    A_row = 0
    for lin in line:
        list = lin.strip('\n').split(' ')
        for i in range(columns):
            A[A_row][i]=float(list[i])

        A_row += 1
        #print(A_row)
    return A


if __name__ == '__main__':
    pwd=os.getcwd()
    intermediate_path=os.path.join(pwd,"Intermediate_parameters")
    intermediate_path = os.path.join(intermediate_path, "Challenge")
    intermediate_path=os.path.join(intermediate_path,"16.1")
    p = rdnumpy(os.path.join(intermediate_path,"16.1A.txt")).tolist()#coefficient matrix
    #print(p[0])
    if len(p[0])==0:
        print("|P|=0, You should excute 06_Clear!")
        sys.exit()
#    print(p[1][1])
    with open(os.path.join(intermediate_path,"16.1_S1.txt"),"r") as file:
        m=int(file.readline())
    with open(os.path.join(intermediate_path,"Set_P.txt"),"r") as file:
        n=int(file.readline())
    #print(m,n)
    # m = 203
    # n = 28
    r=[0 for i in range(m)]
    #print(size(p))
#    print(r[0])
    #print(size(r))

    c = [0 for i in range(n)]#coefficient for Optimization
    c[0]=1

    MODEL = gurobipy.Model("Example")

    # =====Create Variables===========
    x = MODEL.addVars(n, vtype = gurobipy.GRB.BINARY, name='x')

    # ====Update variable environment=====
    MODEL.update()

    # ========Create objective function============
    MODEL.setObjective(0)

    # =======Create constraint conditions=============

    MODEL.addConstrs(x.prod(p[i])== r[i] for i in range(m))

    # ======Execute 0-1 linear programming model=================

    MODEL.Params.PoolSolutions = 50000
    MODEL.Params.PoolSearchMode = 2


    MODEL.optimize()
    cnt_fo_solutions=[]
    print("Obj:", MODEL.objVal)
    fp = open(os.path.join(intermediate_path,"16.1_output.txt") ,"w")
    print(MODEL.SolCount, end="\n",file=fp)
    print(MODEL.SolCount)
    for i in range(MODEL.SolCount):
        MODEL.Params.SolutionNumber = i
        # print("Obj_{} = {}".format(i + 1, MODEL.PoolObjVal))
        for j in MODEL.Xn:
            print("{}".format(int(round(j))), end="",file=fp)
        print("\n", end="",file=fp)

