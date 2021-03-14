

def createRoot(Ladder, A, n):
    if len(A)> 1:
 
         # Finding the mid of the array
        mid = len(A)//2
 
        # Dividing the array elements
        L = A[:mid]
 
        # into 2 halves
        R = A[mid:]
 
        # Sorting the first half
        createRoot(Ladder, L, n)
 
        # Sorting the second half
        createRoot(Ladder, R, n)
 
        i = j = k = 0
 
        # Copy data to temp arrays L[] and R[]
        while i < len(L) and j < len(R):
            if L[i] < R[j]:
                A[k] = L[i]
                i += 1
            else:
                x = L[i]
                row =  - x
                A[k] = R[j]
                j += 1
            k += 1
 
        # Checking if any element was left
        while i < len(L):
            A[k] = L[i]
            i += 1
            k += 1
 
        while j < len(R):
            A[k] = R[j]
            j += 1
            k += 1
 
##end function

def addBar(Ladder, row, col):
    while Ladder[row][col] == 1:
        row -= 1
        col -=1
    Ladder[row][col] = 1


def printLadder(Ladder):
    
    for i in range(len(Ladder)):
        for j in range(len(Ladder[0])):
            print(Ladder[i][j], end=" ")
        print("\n")

def printList(arr):
    print("List is ")
    for i in range(len(arr)):
        print(arr[i], end=" ")
    print()
if __name__ == "__main__":
    A = [3,2,1,5,7,4,6]
    printList(A)
    rows, cols = (2*6 - 1, 6)
    Ladder = [[0 for i in range(cols)] for j in range(rows)]
    createRoot(Ladder, A, len(A))
    printList(A)
    printLadder(Ladder)
