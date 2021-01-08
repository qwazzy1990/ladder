import os
import sys 

globCount = 1
f = open("KInversions.txt", "w")

def kInversions(n, perm, k, l):
    if n <= 0 and k <= 0:
        printPermToFile(perm)
        global globCount
        globCount += 1
        return
    else:
        for i in range(len(l)):
            x = l[i]
            rnk = rank(x, l)
            y = n-rnk
            if y <= k:
                perm[n-1] = x 
                del l[i]
                kInversions(n-1, perm, k - (n - rnk), l)
                l.insert(i, x)



def rank(x, l):
    for i in range(len(l)):
        if l[i] == x:
            return i+1
    
    return -1


def choose(n, r):
    numerator = factorial(n)
    denominator = factorial(r)*factorial(n-r)
    return int(numerator/denominator)


def factorial(n):
    c = 1 
    for i in range(n, 1, -1):
        c *= i
    return c


def resetPerm(perm):
    for i in range(len(perm)):
        perm[i] = 0


def printPermToFile(perm):

    for i in range(len(perm)):
        global f
        print(str(perm[i])+" ", file=f, end='')
    
    print(file=f)

if __name__ == "__main__":

    
    for i in range(choose(5, 2)+1):
        perm = [0,0,0,0,0]
        n = 5 
        k = i
        l = [1,2,3,4,5]
        kInversions(n, perm, k, l)
    f.close()

