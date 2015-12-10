Routing Project
CMSC 481
Edward LaFemina <edlafem1@umbc.edu>


This program is compilied with no additional flags or headers.
gcc -o routing Driver.c


To run, you must have a file named "input.txt" in the same directory as the executable.
The format should be a list of lines such as the following:
<a>$<b>$<cost>

Where <a> and <b> are nodes and <cost> is the cost to get from node a to node b.
 See the below example of a valid line:
1$5$8

NOTE: The program expects that nodes are numbered starting at 1 and not skipping any numbers.
I.e. if there are 5 nodes, they must be labelled 1,2,3,4, and 5. NOT 1,3,7,9,11(skipping numbers), or 2,3,4,5,6(not starting at 1).
To recap: node labels should start at 1 and be consecutive.


The last two lines of the input file must be as follows:
Source$<s>
Destination$<d>

Where <s> is the source node and <d> is the destination node for which a 
path should be calculated.

The output of this program will be in a file called "output.txt".
It will first have a section that displays the source and destination node numbers,
 then the least cost path from source to destination, and a total distance.
The next section will consist of the routing tables at each node. Each table has 
three columns: Destination, Next Hop, and Cost.
Next Hop is the node to which traffic should be sent in order to get to the 
destination from the current node. The cost is the least cost from the current node 
to the Destination node, not the distance from the current node to the Next Hop.

Example input and output files have been provided in this submission.