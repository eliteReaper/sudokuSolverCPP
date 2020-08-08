# A Very Fast Sudoku Solver made in C++

# How to Run ?
- # Compiling
  - Standard: g++ solver.cpp -o solver.exe
  
  - Faster: g++ -O3 solver.cpp -o solver.exe
  
  - Fastest(not recommended): g++ -Ofast solver.cpp -o solver.exe
  
- # Running
  - ./solver.exe


# How did I achieve this speed ?
- To achieve this performance Donalds Knuths Algorithm X was used.
- The data structure used to implement Algo X is Dancing Links also proposed by Donald Knuth.
- I made a significant improvement in performance building the Toroidal Doubly Linked List to represent the puzzle as an exact cover problem. Time Complexity: O(n^3)
- And then the standard backtracking algorithm as described by Algorithm X.

# Tested on a Core i3 7th Gen @ 2.4 Ghz
- 9 X 9 sudoku: 2.029 ms
- 16 X 16: 4.986 ms
- 25 X 25: 15.957 ms
