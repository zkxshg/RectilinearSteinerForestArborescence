#ifndef GRAPHGENERATOR_VECTOR_H    // ȷ��ͷ�ļ�ֻ������һ��
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

// ��������
// ================ GraphGenerator_vector.cpp ================
// ���Լ����ɼ��ڵ�Ԥ����
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
// ================ ���������������ڶԵ㼯N�������� ================
bool compareNodes(const vector<double>& node1, const vector<double>& node2);
void sortNodes(vector<vector<double>>& N);
void printSortN(const vector<vector<double>>& N);

// ================ ���㼯Nת��ΪNode�ṹ ================

vector<Node> convertToNodes(const vector<vector<double>>& N, map<vector<double>, int>& coordinateToId);
void printSortedNodes(const vector<Node>& nodes);

double computeEdgeCost(Node& from, Node& to);
void insertInOrderByCost(vector<int>& ids, int idToInsert, vector<Node>& nodes);
map<int, vector<int>> convertPiToIdMap(vector<Node>& nodes, const map<vector<double>, vector<vector<double>>>& Pi, const map<vector<double>, int>& coordinateToId);
void printIdMap(const map<int, vector<int>>& idMap);

// ��ȡ�릸�еĵ��Ӧ�Ľڵ㼯��
vector<Node*> getOmegaNodes(vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId);
void printOmegaNodes(const vector<Node*>& omegaNodes);

// ��ȡ�����ڦ���N�еĵ�ļ���
vector<Node*> getNonOmegaNodes(const vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId);
void printNonOmegaNodes(const vector<Node*>& nonOmegaNodes);

// ���ƽڵ㼯nodes
vector<Node> deepcopy(const vector<Node>& nodes);

// ����ǰ��׺��ϵ
void copyRelationships(const vector<Node>& nodes, vector<Node>& newNodes);


// ================ ImplicitEnumeration.cpp ������һ�滮 ================
//// === �������� ===
//// ���ڵ㼯�����Ƿ�����ض��ڵ�
//bool containsNode(const vector<Node*>& omegaNodes, Node* node);
//// �������ǰ׺�ķǦ��ڵ�
//void cleanNodesInfeasible(vector<Node>& nodes);
//
//// === Ŀ�꺯������ ===
//// ����Ŀ�꺯��ֵ�������м�֦
//double computeObjectiveFunction_noPrune(const vector<Node>& nodes);
//// ����Ŀ�꺯��ֵ�����м�֦
//double computeObjectiveFunction(vector<Node>& nodes);
//
//// === Լ��������� ===
//// ���Լ������һ�Ƿ����� (����һ�馸�ڵ�)
//bool checkConstraintOne(const vector<Node*>& omegaNodes);
//// ���Լ������һ�Ƿ����� (���ڵ������ڵ�)
//bool checkConstraintOneSingle(Node* node);
//// ���Լ���������Ƿ����� (����һ��N-���ڵ�)
//bool checkConstraintTwo(const vector<Node*>& nodes);
//// ���Լ���������Ƿ����� (���ڵ���N-���ڵ�)
//bool checkConstraintTwoSingle(Node* node);
//
//// === ��ʽ�оٷ���⺯�� ===
//
//// === ��չ�׶� ===
//// ѭ�����ǰ׺�Ƿ���в��ڼ����±�ʱ����Ŀ��ֵ
//double checkConstraintTwoLoop(Node* node, map<int, vector<int>>& idMap, vector<Node>& nodes, double currentObj);
//void expand(vector<Node>& nodes, int k, map<int, vector<int>>& idMap, double currentObj);
//// ������չ�׶�
//double expand_reverse(vector<Node>& nodes, int k, map<int, vector<int>>& idMap, double currentObj);
//
//// === ���ݽ׶� ===
//// ���ǰ���ڵ�
//void clearPredecessor(int end, vector<Node>& nodes);
//// ���ݽ׶�
//void backtrack(vector<Node>& nodes, int l, map<int, vector<int>>& idMap, double currentObj);
//
//// ========== ����ʹ�ӡ���� ==========
//// ��ӡ˹̹����
//void printSteinerArborescence(const vector<Node>& nodes);
//// ��ӡ���ṹ
//void printTree(const vector<Node>& nodes, const Node& currentNode, int level = 0);

#endif  // GRAPHGENERATOR_VECTOR_H
