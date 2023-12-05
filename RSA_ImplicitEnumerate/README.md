# Rectilinear Steiner Forest Arborescence (RSA) - Implicit Enumeration

This repository contains an implementation of the Rectilinear Steiner Forest Arborescence algorithm using implicit enumeration.

## Algorithm Introduction
The Rectilinear Steiner Forest Arborescence algorithm aims to find the optimal arborescence in a graph with rectilinear edge costs, while considering certain constraints. It involves generating test sets, reducing the candidate point set, computing delta values, computing prefix sets, sorting nodes, converting nodes to a Node structure, and performing implicit enumeration to find the optimal solution.

## Files and Functionality
The RSA_ImplicitEnumerate folder contains the following files:

1. `GraphGenerator_vector.h` and `GraphGenerator_vector.cpp`: These files provide functions for generating test sets, reducing the candidate point set, and computing delta values.

2. `GraphConverter.cpp`: This file includes functions for sorting nodes and converting nodes to a Node structure.

3. `main.cpp`: The main entry point of the RSA algorithm implementation. It includes functions for performing implicit enumeration and outputting the optimal solution.

## Usage
To use the RSA algorithm implementation, follow these steps:
1. Ensure that you have a C++ compiler installed on your system.
2. Clone this repository or download the RSA_ImplicitEnumerate folder.
3. Include the necessary files (`GraphGenerator_vector.h`, `GraphGenerator_vector.cpp`, `GraphConverter.cpp`, `main.cpp`) in your C++ project.
4. Compile and run the program using your preferred C++ compiler.

## Complexity Analysis
The time and space complexity of the RSA algorithm implementation can be summarized as follows:
- Time complexity: The overall time complexity of the algorithm is exponential due to the implicit enumeration process. The specific time complexity of each function can be found in the code comments and analysis provided in the README.
- Space complexity: The overall space complexity of the algorithm depends on the size of the input data and the specific implementation details. Please refer to the README for each file's space complexity analysis.

Please note that the complexity analysis provided is based on the code and analysis described in the README. The actual performance may vary depending on the input data and the specific implementation details.

Feel free to modify and enhance the implementation according to your needs. If you have any questions or encounter any issues, please don't hesitate to reach out for assistance.
