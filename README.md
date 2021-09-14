
The code should read a list of number pairs represetating closed ranges from a file called 'ranges.txt'.

It also should read a list of point, namely, 'numbers.dat'.

The function is to find the number of ranges that contain each point.

For an example, a point A is contained inside the interval [X,Y] if X<=A<=Y. The output should maintain the same ordering as the input points (numbers.dat).

ranges.txt
 0 12
 103 201
 13 22
 202 223
 23 35

numbers.dat
2
5
103
24
55
204
106
500
54

output
1
1
1
1
0
1
1
0
0
