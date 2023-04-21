Project 5

Vineeth Konjeti, Amal Sam


Vineeth - Tackled the initial setup and spell word function

Amal - Wrote the early versions of BFS and the main function

We both met up and handled the bug fixing and created the helper functions together mostly on Amal's device




The time complexity of our implementation is o(VE^2). The algorithm can iterate through all the edges (E) in the graph at most V times. In each iteration, 
it takes O(E) time to find the augmenting path using breadth-first search. Since the maximum number of iterations is V, the overall time complexity is O(VE^2).