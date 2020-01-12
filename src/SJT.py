import os
import sys 

class SJT:
    def s_permutations(seq):
        def s_perm(seq):
            if not seq:
                return [[]]
            else:
                new_items = []
                for i, item in enumerate(s_perm(seq[:-1])):
                    if i % 2:
                        # step up
                        new_items += [item[:i] + seq[-1:] + item[i:] for i in range(len(item) + 1)]
                        print("new items is ", new_items)
                    else:
                        # step down
                        new_items += [item[:i] + seq[-1:] + item[i:]
                            for i in range(len(item), -1, -1)]
                return new_items
 
        return [(tuple(item), -1 if i % 2 else 1)
            for i, item in enumerate(s_perm(seq))]

if __name__ == "__main__":

    arr = [1, 2, 3, 4]
    print(arr[:-1])
    items = SJT.s_permutations([1, 2, 3, 4])
    print(items)
