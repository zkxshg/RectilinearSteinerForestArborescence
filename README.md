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

# 直角斯坦纳森林有向树 (RSA) - 隐式列举

该仓库包含了使用隐式列举算法实现的直角斯坦纳森林有向树算法。

## 算法介绍
直角斯坦纳森林有向树算法旨在找到具有直角边成本的图中的最优有向树，同时考虑特定的约束条件。它涉及生成测试集，减少候选点集，计算 δ 值，计算前缀集，对节点进行排序，将节点转换为 Node 结构，以及通过隐式列举找到最优解。

## 文件和功能
RSA_ImplicitEnumerate 文件夹包含以下文件：

1. `GraphGenerator_vector.h` 和 `GraphGenerator_vector.cpp`：这些文件提供了生成测试集、减少候选点集和计算 δ 值的函数。

2. `GraphConverter.cpp`：该文件包含了对节点进行排序和将节点转换为 Node 结构的函数。

3. `main.cpp`：RSA 算法实现的主入口点。它包含了执行隐式列举和输出最优解的函数。

## 使用方法
要使用 RSA 算法实现，请按照以下步骤操作：
1. 确保在您的系统上安装了 C++ 编译器。
2. 克隆该仓库或下载 RSA_ImplicitEnumerate 文件夹。
3. 在您的 C++ 项目中包含所需的文件 (`GraphGenerator_vector.h`、`GraphGenerator_vector.cpp`、`GraphConverter.cpp`、`main.cpp`)。
4. 使用您喜欢的 C++ 编译器编译和运行程序。

## 复杂度分析
RSA 算法实现的时间和空间复杂度可以总结如下：
- 时间复杂度：由于隐式列举过程，算法的整体时间复杂度是指数级的。每个函数的具体时间复杂度可以在代码注释和 README 中找到。
- 空间复杂度：算法的整体空间复杂度取决于输入数据的大小和具体的实现细节。每个文件的空间复杂度分析请参考 README。

时间复杂度：
- generateTestSet(int n, int ub): O(n)
- getSubsets(const vector<vector<double>>& Omega, int size): O(2^n)
- getGLB(const vector<vector<double>>& points): O(n)
- generateN(const vector<vector<double>>& Omega, int d): O(n^2)
- computeDelta(const vector<vector<double>>& N, const vector<vector<double>>& Omega): O(n^2)
- computePi(const vector<vector<double>>& N, const map<vector<double>, double>& delta): O(n^2)
- sortNodes(vector<vector<double>>& N): O(nlogn)
- convertToNodes(const vector<vector<double>>& N, map<vector<double>, int>& coordinateToId): O(n)
- convertPiToIdMap(vector<Node>& nodes, const map<vector<double>, vector<vector<double>>>& Pi, const map<vector<double>, int>& coordinateToId): O(n^2)
- getOmegaNodes(vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId): O(n)
- getNonOmegaNodes(const vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId): O(n)
- deepcopy(const vector<Node>& nodes): O(n)
- copyRelationships(const vector<Node>& nodes, vector<Node>& newNodes): O(n)
- compareNodes(const vector<double>& node1, const vector<double>& node2): O(1)
- insertInOrderByCost(vector<int>& ids, int idToInsert, vector<Node>& nodes): O(n)

空间复杂度：
- generateTestSet(int n, int ub): O(n)
- getSubsets(const vector<vector<double>>& Omega, int size): O(2^n)
- generateN(const vector<vector<double>>& Omega, int d): O(n^2)
- computeDelta(const vector<vector<double>>& N, const vector<vector<double>>& Omega): O(n^2)
- computePi(const vector<vector<double>>& N, const map<vector<double>, double>& delta): O(n^2)
- sortNodes(vector<vector<double>>& N): O(n)
- convertToNodes(const vector<vector<double>>& N, map<vector<double>, int>& coordinateToId): O(n)
- convertPiToIdMap(vector<Node>& nodes, const map<vector<double>, vector<vector<double>>>& Pi, const map<vector<double>, int>& coordinateToId): O(n^2)
- getOmegaNodes(vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId): O(n)
- getNonOmegaNodes(const vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId): O(n)
- deepcopy(const vector<Node>& nodes): O(n)
- copyRelationships(const vector<Node>& nodes, vector<Node>& newNodes): O(n)
- compareNodes(const vector<double>& node1, const vector<double>& node2): O(1)
- insertInOrderByCost(vector<int>& ids, int idToInsert, vector<Node>& nodes): O(n)

