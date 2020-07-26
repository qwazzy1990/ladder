//this algorithm generates all permutations of size N by creating cycles of permutations 
//with 0...k inversions where K = N(N-1)/2. The maximal number of inversions for a permutation of size N.
//author: Patrick Di Salvo
//Date: July 10th 2020

struct perm 
{
    int* perm;
    int numInversions;
};
typedef struct perm Perm;


//creates a permutation with k inversions, the number of elements, E, in the new permutation is 
//equal to the smallest E that satisfies the equation E(E-1)/2 >= k . 
Perm* newPerm(int k);