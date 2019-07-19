# ladder

An Algorithm for enumerating all efficient ladder-lotteries/amida/ghost-legs (three terms for the same thing).
An efficient ladder-lottery is a network of vertical lines, hereby known as lines, and horizontal lines hereby known as bars. The ladder-lottery "randomly" sorts the original permutation into the resulting permutation which is the idendity permutation ordered from 1, 2, 3...,N.  For each element in the original permutation there is exactly 1 line. The line is directly below the position of that element in the permutation. In the program's output, a line is represented by a gap between successive horizontal bars. Or the gap to the leftmost bar or a gap to the right most bar. Therefore a gap in the output is the position of the element in the origial permutation. For each inversion in the original permutation there is exactly 1 bar in an efficient ladder lottery (not true for ladder lotteries in general). An inversion is defined as follows. A greater value in the permutation is to the left of a lesser value in ther permutation. E.g.  1 2 3 = no inversion whereas 1 3 2 = 1 inversion. In the program's output a bar looks like this, [m n], where m and n are non 0 integers. m and n are the two values in the original permutation that cause an inversion of the permutation. These two values in the original permutation get swapped along said bar when they travel down the ladder lottery. Another way of thinking about it is that an element travles down a line until it comes to an end point of bar and then crosses the bar to the next line either to the left or right of the line it was just travelling down. The output also displays [0 0] which look like bars. However, the [0 0] means that there is no bar in that location. 


Langauges used - C.

To run: 

Go to the folder "ladder".
Type "make" in the terminal.
Program will prompt you.
Enter a permutation.
Hit "enter".
Done.

Example of a ladder from the following permutation:

4321

4  3  2  1
Clean Level:1
Level 0
Ladder Number:1
[4 3] [0 0] [0 0]
[0 0] [4 2] [0 0]
[3 2] [0 0] [4 1]
[0 0] [3 1] [0 0]
[2 1] [0 0] [0 0]
[0 0] [0 0] [0 0] 

Interpret the output as follows - gaps in output are lines as defined above. 
                                - [m n] where m and n > 0 in output are bars as defined above.

4    3     2     1  
[4 3] [0 0] [0 0]
[0 0] [4 2] [0 0]
[3 2] [0 0] [4 1]
[0 0] [3 1] [0 0]
[2 1] [0 0] [0 0]
[0 0] [0 0] [0 0]
1     2     3     4

Take element 4 in the original permutation, It starts at the gap to the left of the left most collumn/collumn 1, goes down said gap, travels across [4 3], goes down the gap between collumn 1 and 2, crosses the bar [4 2], goes down the gap between collumns 2 and 3, crosses [4 1] then goes down the gap to the right of collumn 3 until it hits the bottom of said gap.
