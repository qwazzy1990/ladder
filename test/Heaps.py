


def Heaps(k, perm):
    if k == 1:
        print(perm)
        return

    else:
        for i in range(k):
            Heaps(k-1, perm)
            if k %2 == 0:
                perm[i], perm[k-1] = perm[k-1], perm[i]
            else:
                perm[0], perm[k-1] = perm[k-1], perm[0]



if __name__ == "__main__":
    
    l = []
    for i in range(3):
        l.append(i+1)

    Heaps(len(l), l)


