# A Very Fast Sudoku Solver made in C++

# How to Run ?
- # Compiling
  - Standard: g++ solver.cpp -o solver.exe
  
  - Faster: g++ -O3 solver.cpp -o solver.exe
  
  - Fastest(not recommended): g++ -Ofast solver.cpp -o solver.exe
  
- # Running
  - Put your sudoku puzzle in puzzle.txt, use numbers to represent given data and 0(Zero) to represent missing data with spaces in between every number.
  - Also put the size of the sudoku on the first line(look at examples in /puzzles)
  - Execute ./solver.exe in the terminal while in the root directory of the project.


# How did I achieve this speed ?
- To achieve this performance Donalds Knuths Algorithm X was used.
- The data structure used to implement Algo X is Dancing Links also proposed by Donald Knuth.
- I made a significant improvement in performance building the Toroidal Doubly Linked List to represent the puzzle as an exact cover problem. Time Complexity: O(n^3)
- And then the standard backtracking algorithm as described by Algorithm X.

# Tested on a Core i3 7th Gen @ 2.4 Ghz
- 9 X 9 sudoku: 2.029 ms
- 16 X 16: 4.986 ms
- 25 X 25: 15.957 ms

# Will be adding a GUI soon
