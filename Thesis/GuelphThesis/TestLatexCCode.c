
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//ladder data structure
typedef struct ladder
{
    int** ladder;
    int numRows;
    int numCols;
    int numBars;
} Ladder;

//initialize the ladder 
Ladder* initLadder(int n);

//returns the coordinates of the bar or absence of a bar formed between x and y
int* GetCoordinates(int n, int x, int posX, int posY);
//creates a copy of a permutation
int* copyPerm(int* og, int start, int end);
//creates a copy of og permutation minus the element at index
int* removeElement(int* og, int index, int n);
//creates the canonical ladder
void CreateCanonical(Ladder* l, int* pi, int n);
//prints the ladder
void printLadder(Ladder *l);

//Gets the coordinates of the row and coulumn for ModifiedSJT
int* GetCoordinatesTwo(int* rX, int n, int x, int posX, int posY);


//modified SJT algorithm for listing Ln
void ModifiedSJT(Ladder* l, int x, int n, bool* direction, int* rX);


//initializes the ladder data structure
//n is the length of a permutation
Ladder* initLadder(int n)
{
	Ladder* l = calloc(1, sizeof(Ladder));
	int nCols = n-1;
	int nRows = 2*(n-1) - 1;
	int nBars = 0;
	l->ladder = calloc(nRows, sizeof(int*));
	for(int i = 0; i < nRows; i++)l->ladder[i] = calloc(nCols, sizeof(int));
	l->numRows = nRows;
	l->numCols = nCols;
	l->numBars = nBars;
	return l;
}//end function

/**
	Returns the coordinates corresponding to a bar or absence of a bar 
	formed between elements x and y which lies on the associated diagonal of 'x'. 
	Parameter conditions: n is the size of a permutation, x is the element whose diagonal we are populating, posX is the position of x in pi, 
							posY is the position of 'y' in pi. posX != posY and x > y
    Post conditions: The (row, column) coordinates of the bar or absence of a bar corresponding to the two elements x and y
**/ 

int* GetCoordinates(int n, int x, int posX, int posY){
	
	int* coordinates = calloc(2, sizeof(int));
	//if x is to the right of y, then they form the absence of an inversion. Use 
	//equation 3.1.1 from Equation 3.1
	if(posX > posY){
		int row = (2*n - x - 1) - (x - posY)+ 1;
		int col = (x - 1) - (x - posY) + 1;

		//subtract 1 for 0 indexing
		coordinates[0] = row ;
		coordinates[1] = col;
	}//end If
	//else if posX is < posY, then they form an inversion. Use equation 3.1.2 from Equation 3.1
	else{
		int row = (2*n - x - 1) - (x - posY);
		int col = (x - 1) - (x - posY);
		coordinates[0] = row;
		coordinates[1] = col;
	}//end else 

	return coordinates;

}//end function

//creates a copy of a permutation from a start index to an end index
int* copyPerm(int* og, int start, int end){
	
	int size = (end - start);
	int* copy = calloc(size, sizeof(int));
	for(int i = start; i < end; i++){
		copy[i - start] = og[i];
	}
	return copy;
}

int* removeElement(int* og, int index, int n){

	int* leftPart = copyPerm(og, 0, index);
	int* rightPart = copyPerm(og, index+1, n);
	int size = n-1;
	int* newPerm = calloc(n-1, sizeof(int));
	for(int i = 0; i < index; i++)newPerm[i] = leftPart[i];
	for(int i = 0; i < n-index-1; i++)newPerm[index+i] = rightPart[i];
	free(leftPart);
	free(rightPart);
	return newPerm;
}

/**
Sets ladder l to the Canical ladder. 
Parameter conditions: l is created using initLadder, pi is a permutation of order n
Post conditions: l is set to the canonical ladder corresponding to pi
**/
void CreateCanonical(Ladder* l, int* pi, int n)
{
	int x = n;
	int* pi2 = copyPerm(pi, 0, n);
	//from element x=n down to element x=2
	while(x > 1)
	{
		int index = -1;
		//get the index of element 'x' in pi
		for(int i = 0; i < x; i++){
			if(pi2[i] == x){
				index = i;
				break;
			}//end if
		}//end for

		//populate the associated diagonal of x with 1s and 0s.
		for(int i = 0; i < x; i++){
			if(i != index){
				int* coordinates = GetCoordinates(n, x, index, i);
				int row = coordinates[0];
				int col = coordinates[1];
				if(i < index){
                    l->ladder[row][col] = 0;

					
				}else{
                    l->ladder[row][col] = 1;
					l->numBars++; 
				}
			}
		}//end for
		pi2 = removeElement(pi2, index, x);
        x--;


	}//end while
}

void printLadder(Ladder *l)
{
   
    int n = l->numCols + 1;
    int offset = 2*(n-1) - 1;
    printf("\n\n");
    for (int i = 0; i < offset; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            printf("|");
            if (l->ladder[i][j] == 1)
                printf("-----");
            else
                printf("     ");
        }
        printf("|");
        printf("\n");
    }
}


int* GetCoordinatesTwo(int* rX, int n, int x, int posX, int posY)
{
	int* coordinates = calloc(2, sizeof(int));
	//if posX is to the right of posY then x and y are currently 
	//uninverted and will be inverted. Therefore, we add a bar along the associated diagonal of 'x'
	if(posX == posY + 1)
	{
		coordinates[0] = (2*n - x - 1) - (*rX)-1;
		coordinates[1] = (x-1) - (*rX)-1;
		(*rX)++;

	}
	//else if x is to the left of y by 1 position, then x and y are currently inverted and will become 
	//uninverted. Therefore, we remove a bar along the associated diagnoal of 'x'
	else if(posX == posY - 1)
	{

		coordinates[0] = (2*n - x) - (*rX)-1;
		coordinates[1] = x - (*rX)-1;
		(*rX)--;

	}
	//else error occured
	else 
	{
		printf("Error occurred\n");
		exit(0);
	}

	return coordinates;
}

void ModifiedSJT(Ladder* l, int x, int n, bool* direction, int* rX)
{
	if(x > n)
	{
		if(l->numBars == 0)printLadder(l);
		return;
	}
	for(int i = 0; i < x; i++)
	{
		if(i == 0)ModifiedSJT(l, x+1, n, direction, rX);
		else 
		{
			//adding to the associated diagonal of x
			if(direction[x-1] == false)
			{
				int* coordinates = GetCoordinatesTwo(&(rX[x-1]), n, x, 1, 0);
				//subtract 1 for 0 index
				int row = coordinates[0];
				int column = coordinates[1];

				l->ladder[row][column] = 1;
				l->numBars++;
				//printf("Adding a bar row is %d column is %d x is %d RX is %d\n", row, column, x, rX[x-1]);

			}
			//removing a bar from the associated diagonal of x
			else{
				int* coordinates = GetCoordinatesTwo(&(rX[x-1]), n, x, 0, 1);
				int row = coordinates[0];
				int column = coordinates[1];
				//printf("Removing a bar row is %d column is %d x is %d RX is %d\n", row, column, x, rX[x-1]);


				l->ladder[row][column] = 0;
				l->numBars--;	
			}
			printLadder(l);
			ModifiedSJT(l, x+1, n, direction, rX);


		}
	}
	direction[x-1] = !(direction[x-1]);
}


//testing value
int main(int argc, char* argv[])
{
    //testing permutation
    int pi[] = {4,1,7,2,3,6,5};
    int n = 5;
    Ladder* l = initLadder(n);
   

	int* rX = calloc(n, sizeof(int));
	bool* direction = calloc(n, sizeof(int));
	for(int i = 0; i < n; i++){
		direction[i] = false;
		rX[i] = 0;
	}
	ModifiedSJT(l, 2, n, direction, rX);
}
