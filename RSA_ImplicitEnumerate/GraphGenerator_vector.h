#ifndef GRAPHGENERATOR_VECTOR_H    // 确保头文件只被包含一次
#define GRAPHGENERATOR_VECTOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>
#include <random>
#include <unordered_map>
#include <stack>
#include <fstream>
#include <string>


using namespace std;

struct Node {
    int id;
    bool isOmega;
    vector<double> coordinates;
    Node* predecessor;
    set<Node*> successors;
};

// 函数声明
// ================ GraphGenerator_vector.cpp ================
// 测试集生成及节点预处理
vector<vector<double>> generateN(const vector<vector<double>>&, int);
vector<double> getGLB(const vector<vector<double>>&);
vector<vector<vector<double>>> getSubsets(const vector<vector<double>>&, int);
vector<vector<double>> generateTestSet(int n, int ub);
void printSubset(const vector<vector<int>>& subset);
void printN(const vector<vector<double>>& Omega, const vector<vector<double>>& N);
bool lessThanOrEqual(const vector<double>& p, const vector<double>& q);
double manhattanDistance(const vector<double>& p, const vector<double>& q);
map<vector<double>, double> computeDelta(const vector<vector<double>>& N, const vector<vector<double>>& Omega);
void printDelta(const map<vector<double>, double>& delta);
map<vector<double>, vector<vector<double>>> computePi(const vector<vector<double>>& N, const map<vector<double>, double>& delta);
void printPi(const map<vector<double>, vector<vector<double>>>& Pi);
vector<vector<double>> reduceN(const vector<vector<double>>& N, const vector<vector<double>>& Omega, int d);
void printReducedN(const vector<vector<double>>& reducedN);

// ================ GraphConverter.cpp ================
// ================ 定义排序函数，用于对点集N进行排序 ================
bool compareNodes(const vector<double>& node1, const vector<double>& node2);
void sortNodes(vector<vector<double>>& N);
void printSortN(const vector<vector<double>>& N);

// ================ 将点集N转化为Node结构 ================

vector<Node> convertToNodes(const vector<vector<double>>& N, map<vector<double>, int>& coordinateToId);
void printSortedNodes(const vector<Node>& nodes);

double computeEdgeCost(Node& from, Node& to);
void insertInOrderByCost(vector<int>& ids, int idToInsert, vector<Node>& nodes);
map<int, vector<int>> convertPiToIdMap(vector<Node>& nodes, const map<vector<double>, vector<vector<double>>>& Pi, const map<vector<double>, int>& coordinateToId);
void printIdMap(const map<int, vector<int>>& idMap);

// 获取与Ω中的点对应的节点集合
vector<Node*> getOmegaNodes(vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId);
void printOmegaNodes(const vector<Node*>& omegaNodes);

// 获取不属于Ω的N中的点的集合
vector<Node*> getNonOmegaNodes(const vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId);
void printNonOmegaNodes(const vector<Node*>& nonOmegaNodes);

// 复制节点集nodes
vector<Node> deepcopy(const vector<Node>& nodes);

// 复制前后缀关系
void copyRelationships(const vector<Node>& nodes, vector<Node>& newNodes);


// ================ ImplicitEnumeration.cpp 构建零一规划 ================
//// === 辅助函数 ===
//// 检查节点集合中是否包含特定节点
//bool containsNode(const vector<Node*>& omegaNodes, Node* node);
//// 清理仅有前缀的非Ω节点
//void cleanNodesInfeasible(vector<Node>& nodes);
//
//// === 目标函数计算 ===
//// 计算目标函数值，不进行剪枝
//double computeObjectiveFunction_noPrune(const vector<Node>& nodes);
//// 计算目标函数值，进行剪枝
//double computeObjectiveFunction(vector<Node>& nodes);
//
//// === 约束条件检查 ===
//// 检查约束条件一是否满足 (对于一组Ω节点)
//bool checkConstraintOne(const vector<Node*>& omegaNodes);
//// 检查约束条件一是否满足 (对于单个Ω节点)
//bool checkConstraintOneSingle(Node* node);
//// 检查约束条件二是否满足 (对于一组N-Ω节点)
//bool checkConstraintTwo(const vector<Node*>& nodes);
//// 检查约束条件二是否满足 (对于单个N-Ω节点)
//bool checkConstraintTwoSingle(Node* node);
//
//// === 隐式列举法求解函数 ===
//
//// === 扩展阶段 ===
//// 循环检查前缀是否可行并在加入新边时更新目标值
//double checkConstraintTwoLoop(Node* node, map<int, vector<int>>& idMap, vector<Node>& nodes, double currentObj);
//void expand(vector<Node>& nodes, int k, map<int, vector<int>>& idMap, double currentObj);
//// 反向扩展阶段
//double expand_reverse(vector<Node>& nodes, int k, map<int, vector<int>>& idMap, double currentObj);
//
//// === 回溯阶段 ===
//// 清除前驱节点
//void clearPredecessor(int end, vector<Node>& nodes);
//// 回溯阶段
//void backtrack(vector<Node>& nodes, int l, map<int, vector<int>>& idMap, double currentObj);
//
//// ========== 输出和打印函数 ==========
//// 打印斯坦纳树
//void printSteinerArborescence(const vector<Node>& nodes);
//// 打印树结构
//void printTree(const vector<Node>& nodes, const Node& currentNode, int level = 0);

#endif  // GRAPHGENERATOR_VECTOR_H
