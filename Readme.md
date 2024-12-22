#  Description for our source code

---

## Introduction

&emsp;&emsp;We use **C++ & Gurobi（python）** to implement our **IAC**(Improved Approximation Calculation) algorithm, which is about 10 times faster than purely using python. There are 7 program files in total, i.e.:  

​	**00_Makefolders.cpp** is to create necessary folders.
​	**01_Algorithm4.cpp** is Algorithm 4 as is shown in the paper. 
​	**02_Generate_S1_Set** is to Select the 200 points with the highest absolute spectral values in the part-Walsh spectrum $S$  to construct the set $S_1$ ,whose location set is $X_1$. You can adjust according to your computing power.
​	**03_Generate_matrix_for_Gurobi.cpp** is to generate the matrix $\mathbf{H_n}^{\boldsymbol{f_1}}{({X_1},{P})}$, where $f$ is an approximate solution.
​	**04_Solving_with_Gurobi.py** is to find the approximate solutions that satisfies the necessary condition, by using Gurobi to solve formula (9) in the paper.
​	**05_Change_Gurobi_output_to_realsolutions.cpp** is to change the output of **04_Solving_with_Gurobi.py** to the real solutions.
​	**06_Clear_repeate_realsolutions.cpp** is to remove duplicate real solutions from the output results.

​	Number represents the order of execution.

## Restriction

​	In the program files, numbers **00, 01, 02, and 03** must be executed on **Linux** system, while **04, 05, and 06** must be executed under on  **Windows** system. Therefore, you need to create a project for Linux and Windows, respectively. After executing 00, 01, and 02 in the project file on  Linux system, copy folders Problem, Intermediate_parameters, and Results to project file on  Windows system to conduct 04, 05, and 06.

---

# Description of files

---

​	The folders generated during program execution include (taking the open challenge 13.1 as an example): 

## Problem

​	Store the input of **IAC** algorithm, i.e. the set $S$.  The first line of the 13.1.txt file should be $|S|$.

## Intermediate_parameters

​	Store the Intermediate variables for **IAC** algorithm, including:

​	**Set_P.txt** : The set $P$ mentioned in the paper. The first line of the file is $|P|$.
​	**One_approximate_solution.txt** : The truth table of an approximate solution $f_2$.
​	**13.1A.txt** : The matrix $- 2\mathbf{H_n}^{\boldsymbol{f_2}}{({X_1},{P})}$ mentioned in the paper. 
​	**13.1_S1.txt** : The set $S_1$ ,whose location set is $X_1$.
​	**13.1_output.txt** The output of Gurobi, need further enumeration and verification.

## Results

​	**Final_solutions.txt** : Truth tables for Boolean functions satisfying $S$ and there may be duplication among them.
​	**Final_solutions_clear.txt** : Truth tables for Boolean functions satisfying $S$ without duplication. The output of **IAC** algorithm.

---

# Environment required

​	Gurobi (python)  >=10.0.1
​	python: version >= 3.7
​	g++ (Linux) >=11.4.0	
​	gmp(Linux)>=6.3.0	
​	TDM -GCC (Windows) >= 4.9.2

​	Besides, as we need to execute both on Linux and Windows, we strongly recommend using WSL(version >=2) on windows and use VSCode for connection to Linux.

