
ID3_Project 5/04/2015

The goal of this project is to implement a program for inferring imperfect decision trees.

The dataset is described in a file containing only integer numbers. The first number in the file is m, the number of instances; the second is n, the number of features. These two numbers are followed by mn feature values. To simplify the project we will assume that all attributes are Boolean, represented by “0” or “1”. The last attribute is taken as the target attribute.

A partition is described in a text file, containing only integer numbers. For a situation where there are m instances, a partition is described by a sequence of numbers in the range 1 . . . m.
The first number in each partition is the partition id, and each partition is specified in a separate line

The program should read as input one dataset file and one partition file. It should determine which partition to split, and according to what attribute. The program should then produce as output a new partition file.

to compile in UNIX

g++ -o ID3_Test ID3.cpp Storage.cpp

to run

./ID3_Test
