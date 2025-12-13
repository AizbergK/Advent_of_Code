import numpy as np
from scipy.optimize import linprog

def runProgram(fileName):
    with open(fileName, 'r') as f:
        lines = f.readlines()

    result = np.int64(0)
    for i, line in enumerate(lines):
        line = line.strip().split(' ')

        results = line[-1][1:-1].split(',')
        buttons = line[1:-1]

        for i in range(len(buttons)):
            buttons[i] = buttons[i][1:-1].split(',')

        eqMat = np.zeros((len(results), len(buttons)), dtype=int)
        for i in range(len(buttons)):
            for index in buttons[i]: 
                eqMat[int(index), i] = 1

        col = np.array(results, dtype=int)

        res = linprog(
            c=np.ones(eqMat.shape[1]),
            A_eq=eqMat,
            b_eq=col,
            bounds=[(0, None)] * eqMat.shape[1],
            integrality=np.ones(eqMat.shape[1], dtype=int),
            method="highs"
        )

        result += np.sum(res.x)
    print(f"{result:.0f}")

runProgram("../../other/test.txt")
runProgram("../../other/input.txt")