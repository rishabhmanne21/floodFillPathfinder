# floodFillPathfinder
This project consists of two parts; a flood fill algorithm that populates a dynamically allocated 2D array and a path finder that tracks the path from a starting position to an ending position.



Dynamic Allocation of 2D Array:

First, I received the dimensions of the 2D array from the user, which I used to create an empty array. Then, I dynamically allocated the memory for the array. This was accomplished in two steps.

- I created a space for each row in the array
- For each row in the array, I created a space for a column(this ensured that a segmentation fault wouldn't occur)


Flood-Fill Algorithm:

I used BFS for this algorithm because the implementation of queues not only makes the process easier, but allows me to track the Moore Neighborhood values much more efficiently. Furthermore, I created an array that holds the potential coordinates that the algorithm could move to from its central point.

- Pushing our x and y coordinates into the queue
- Pop the first two coordinates and store them in variables
- Using the isAvailable method to see if all the neighbors are valid until all neighborhood values are covered
- If not, we move on to another center and repeat the process until there is nothing in the queue(implies all values of array are covered)


Pathfinder Algorithm:

The user would've inputted a starting and ending position and this algorithm traces the step by placing a '*' at every step.

- Same algorithm as above, however, checked for the minimum value(across all neighborhood values) along with its availability
- Then the new minimum will be the center, repeat
- This should lead the way to the starting point
- If there is no path, then an unsuccessful message will be printed to the user

At the end, the updated pathfinder array and floodFill array are outputted to an output file called "wave.txt" and a success message is printed at the end.
