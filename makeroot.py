import os 
import sys 



def insert(ladder, row, col):
    ladder[row][col] = 1
    

def printLadder(ladder):
    for i in range(0, len(ladder)):
        for j in range(0, len(ladder[0])):
            print(str(ladder[i][j]) + " ", end="")
        print("\n")

def makeRoot(ladder, pi, prevRow, n):
    if n == 1:
        return
    if max(pi) == n:
        idx = pi.index(n)
        numBars = (len(pi) - idx) - 1
        col = n-2
        row = numBars - 1
        for i in range(0, numBars):
            insert(ladder, row, col)
            row-=1
            col-=1

        makeRoot(ladder, pi, numBars-1, n-1)
    else:
        idx = pi.index(n)
        row = prevRow+1
        c1 = row
        col = n-2
        for i in range(idx, len(pi)):
            if pi[idx] > pi[i]:
                insert(ladder, c1, col)
                c1 -=1
                col -=1
        makeRoot(ladder, pi, row, n-1)
        


def initLadder(n):
    cols = n-1
    rows = 2*(n-1) - 1
    ladder = [[0 for i in range(0, cols)]for j in range(0, rows)]
    return ladder

if __name__ == "__main__":
    pi = [5,7,3,4,1,2,6]
    ladder = initLadder(7)
   
    makeRoot(ladder, pi, 0, max(pi))
    printLadder(ladder)
