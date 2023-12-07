#include "GraphGenerator_vector.h"

// ================ 构建零一规划 ================
double bestObjective = INT_MAX;
map<int, int> prefixIndex;
vector<Node> optimalNodes;

// 检查节点集合中是否包含node
bool containsNode(const vector<Node*>& omegaNodes, Node* node) {
    for (const auto& n : omegaNodes) {
        if (n == node) {
            return true;
        }
    }
    return false;
}

// 清理仅有前缀的非Ω节点
void cleanNodesInfeasible(vector<Node>& nodes) {
    for (Node& node : nodes) {
        if (!node.isOmega) {
            // 如果一个节点只有前驱节点但没有后继节点，我们清除其前驱节点并清空其后继节点列表
            if (node.predecessor && node.successors.empty()) {
                node.predecessor = nullptr;
                node.successors.clear();
            }
        }
    }
}

// 计算节点集nodes的objective function: min ∑_(i,j)∈A c_ij x_ij  (4.1)
double computeObjectiveFunction_noPrune(const vector<Node>& nodes) {
    double cost = 0.0;
    for (const Node& node : nodes) {
        if (node.predecessor) {
            cost += abs(node.coordinates[0] - node.predecessor->coordinates[0]);
            cost += abs(node.coordinates[1] - node.predecessor->coordinates[1]);
        }
        for (const Node* successor : node.successors) {
            cost += abs(node.coordinates[0] - successor->coordinates[0]);
            cost += abs(node.coordinates[1] - successor->coordinates[1]);
        }
    }
    return cost;
}

double computeObjectiveFunction(vector<Node>& nodes) {

    double cost = 0;

    for (Node& n : nodes) {
        Node* node = &n;

        if (!node->isOmega) {

            if (node->predecessor && node->successors.empty()) {
                // 非Ω点只有入边没有出边,清空前缀和出边
                node->predecessor = nullptr;
                node->successors.clear();

                cout << "Clear Node" << node->id << endl;

                continue;
            }
        }

        // 添加node的边的cost
        for (Node* successor : node->successors) {
            cost += computeEdgeCost(*node, *successor);
        }

    }

    return cost;
}

// === 检查约束条件一是否满足: ∑_h∈P_i x_hi ≥ 1, i ∈ Ω, i ≠ 0  (4.2) ===
bool checkConstraintOne(const vector<Node*>& omegaNodes) {
    for (const auto& node : omegaNodes) {
        if (node->predecessor == nullptr) {
            return false;
        }
    }
    return true;
}

// 检查属于Ω的单点
bool checkConstraintOneSingle(Node* node) {
    if (node->predecessor == nullptr) {
        return false;
    }
    return true;
}

// === 检查约束条件二是否满足: |S_i|∑_h∈P_i x_hi - ∑_j∈S_i x_ij ≥ 0, i ∈ N - Ω, i ≠ 0  (4.3) ===
bool checkConstraintTwo(const vector<Node*>& nodes) {
    for (const auto& node : nodes) {
        if (node->successors.empty()) {
            // 无出边，左侧必须为0
            if (node->predecessor != nullptr) {
                return false;
            }
        }
        else {
            // 有出边，左侧必须为1
            if (node->predecessor == nullptr) {
                return false;
            }
        }
    }
    return true;
}

// 检查属于 N - Ω的单点
bool checkConstraintTwoSingle(Node* node) {
    if (!node->successors.empty()) {
        // 有出边，左侧必须为1
        if (node->predecessor == nullptr) {
            return false;
        }
    }
    return true;
}

bool checkConstraintTwoSingle_old(Node* node) {
    if (node->successors.empty()) {
        // 无出边，左侧必须为0
        if (node->predecessor != nullptr) {
            return false;
        }
    }
    else {
        // 有出边，左侧必须为1
        if (node->predecessor == nullptr) {
            return false;
        }
    }
    return true;
}


// ================ 隐式列举法求解零一规划 ================
// 循环检查前缀是否可行并在加入新边时更新目标值
double checkConstraintTwoLoop(Node* node, map<int, vector<int>>& idMap, vector<Node>& nodes, double currentObj) {
    double obj = currentObj; // 使用传入的目标函数值

    while (!checkConstraintTwoSingle(node) && node->id != 0) {
        // 找到最小cost的入边作为前驱
        int minCostPrefixId = idMap[node->id][prefixIndex[node->id]];
        Node* minCostPrefixNode = &nodes[minCostPrefixId];
        node->predecessor = minCostPrefixNode;

        // 将当前节点添加到前驱节点的后继节点列表中
        minCostPrefixNode->successors.insert(node);

        // 计算新边的cost并加到目标函数值上
        obj += computeEdgeCost(*node, *minCostPrefixNode);

        // 更新当前节点为前驱节点，继续检查约束条件
        node = minCostPrefixNode;
    }

    // 返回修改后的目标函数值
    return obj;
}

// === 扩展阶段 ===
void expand(vector<Node>& nodes, int k, map<int, vector<int>>& idMap, double currentObj) {
    Node* node = &nodes[k];

    // 检查约束条件，针对不可行设置前驱节点
    if (node->isOmega) {
        // node属于Ω，检查约束条件一
        if (!checkConstraintOneSingle(node)) {
            // 找最小cost的入边作为前驱 
            int minCostPrefixId = idMap[node->id][prefixIndex[node->id]];
            Node* minCostPrefixNode = &nodes[minCostPrefixId];
            node->predecessor = minCostPrefixNode;

            // 将当前节点添加到前驱节点的后继节点列表中
            minCostPrefixNode->successors.insert(node);

            // 计算新边的cost并加到目标函数值上
            currentObj += computeEdgeCost(*node, *minCostPrefixNode);

            // 检查约束条件二
            node = minCostPrefixNode;
            currentObj = checkConstraintTwoLoop(node, idMap, nodes, currentObj);
        }
    }
    else {
        // 检查约束条件二
        currentObj = checkConstraintTwoLoop(node, idMap, nodes, currentObj);
    }

    // 如果目标函数的值超过当前最优解，立即返回
    if (currentObj > bestObjective) {
        return;
    }

    // 如果还有未处理的节点，继续扩展
    if (k < int(nodes.size()) - 1) {
        expand(nodes, k + 1, idMap, currentObj);
    }
    else {
        // 更新最优解
        bestObjective = computeObjectiveFunction(nodes);
        cout << bestObjective << ":" << currentObj << endl;
    }
}

// 扩展阶段-逆向
double expand_reverse(vector<Node>& nodes, int k, map<int, vector<int>>& idMap, double currentObj) {
    Node* node = &nodes[k];

    if (node->isOmega && k != 0) {
        // 检查约束条件一
        if (!checkConstraintOneSingle(node)) {
            // 找最小cost的入边作为前驱
            int minCostPrefixId = idMap[node->id][prefixIndex[node->id]];
            Node* minCostPrefixNode = &nodes[minCostPrefixId];
            // 更新当前节点的前缀
            node->predecessor = minCostPrefixNode;
            // 将当前节点加入前缀节点的后缀集合
            minCostPrefixNode->successors.insert(node);
            //  更新 ObjectiveValue
            currentObj += computeEdgeCost(*node, *minCostPrefixNode);
        }
    }
    else if (k > 0) {
        // 检查约束条件二
        if (!checkConstraintTwoSingle(node)) {
            // 找最小cost的入边作为前驱
            int minCostPrefixId = idMap[node->id][prefixIndex[node->id]];
            Node* minCostPrefixNode = &nodes[minCostPrefixId];
            // 更新当前节点的前缀
            node->predecessor = minCostPrefixNode;
            // 将当前节点加入前缀节点的后缀集合
            minCostPrefixNode->successors.insert(node);
            //  更新 ObjectiveValue
            currentObj += computeEdgeCost(*node, *minCostPrefixNode);
        }
    }

    // cout << node->id << ":" << node->isOmega << "," << node->predecessor << endl;

    if (currentObj > bestObjective) {
        return currentObj;
    }

    if (k > 0) {
        return expand_reverse(nodes, k - 1, idMap, currentObj);
    }
    else {
        // 对于k = 0的情况，只更新最优解
        if (bestObjective > currentObj) {
            cout << bestObjective << ":" << currentObj << endl;
            copyRelationships(nodes, optimalNodes);  // 更新最优节点配置

        }
        bestObjective = min(bestObjective, currentObj);
    }

    return currentObj;
}

// === 回溯阶段 ===

// 清理 (0, end] 的所有节点相互的前后缀
void clearPredecessor(int end, vector<Node>& nodes) {
    for (int i = end; i > 0; i--) {
        // 清除当前节点的前缀和对应前缀节点的后缀
        if (nodes[i].predecessor != nullptr) {
            nodes[i].predecessor->successors.erase(&nodes[i]);
            nodes[i].predecessor = nullptr;
        }

        // 重置当前前缀索引
        prefixIndex[i] = 0;
    }

}

// 进行递归回溯
void backtrack(vector<Node>& nodes, int l, map<int, vector<int>>& idMap, double currentObj) {
    // cout << "Backtrack " << l << ":" << endl;

    if (!nodes[l].isOmega && nodes[l].successors.empty()) {
        backtrack(nodes, l - 1, idMap, currentObj);
        return;
    }

    if (l > 0) {
        vector<int> prefixes = idMap[l];
        // int cou = 0;  cout << "===========" << cou++ << "===========" << endl;
        for (int p : prefixes) {
            // 清理从l之前的所有节点之间的边
            clearPredecessor(l - 1, nodes);

            // 设置前驱和对应节点的后缀
            nodes[l].predecessor = &nodes[p];
            nodes[p].successors.insert(&nodes[l]);

            // 计算ObjCost
            double preObj = currentObj + computeEdgeCost(nodes[l], nodes[p]);

            if (preObj < bestObjective) {
                // 更新objCost
                double obj = expand_reverse(nodes, l - 1, idMap, preObj);

                // 递归扩展
                backtrack(nodes, l - 1, idMap, preObj);
            }

            // 重置前驱 
            nodes[p].successors.erase(&nodes[l]);
            nodes[l].predecessor = nullptr;
        }

    }

}

// 输出构建的RSA
void printSteinerArborescence(const vector<Node>& nodes) {
    for (const auto& node : nodes) {
        // 输出节点的信息
        cout << "Node ID: " << node.id << endl;
        cout << "Coordinates: (" << node.coordinates[0] << ", " << node.coordinates[1] << ")" << endl;

        // 输出前驱节点的信息
        if (node.predecessor != nullptr) {
            cout << "Predecessor ID: " << node.predecessor->id << endl;
            cout << "Predecessor Coordinates: (" << node.predecessor->coordinates[0] << ", " << node.predecessor->coordinates[1] << ")" << endl;
        }

        // 输出后继节点的信息
        if (!node.successors.empty()) {
            cout << "Successor IDs:";
            for (const auto& successor : node.successors) {
                cout << " " << successor->id;
            }
            cout << endl;
        }

        cout << endl;
    }
}

void printTree(const vector<Node>& nodes, const Node& currentNode, int level = 0) {
    // 输出当前节点的信息及层级
    cout << string(level, '\t') << "Node ID: " << currentNode.id; if (currentNode.isOmega) cout << "*"; cout << endl;
    cout << string(level, '\t') << "Coordinates: (" << currentNode.coordinates[0] << ", " << currentNode.coordinates[1] << ")" << endl;

    // 输出当前节点的后继节点的信息
    cout << string(level, '\t') << "Successor IDs:";
    for (const auto& successor : currentNode.successors) {
        cout << " " << successor->id;
    }
    cout << endl;

    // 递归输出后继节点的子树
    for (const auto& successor : currentNode.successors) {
        printTree(nodes, *successor, level + 1);
    }
}

// 将结构写入csv文件
void saveNodesToCSV(vector<Node> nodes, const std::string& filename) {
    std::ofstream file(filename);

    // Write header
    file << "id,isOmega,";
    for (size_t i = 0; i < nodes[0].coordinates.size(); ++i) {
        file << "coordinate_" << i << ",";
    }
    file << "predecessor_id,successor_ids\n";

    // Write data
    for (Node node : nodes) {
        // Redundant Nodes
        if (!node.isOmega && node.predecessor == nullptr && node.successors.empty()) {
            continue;
        }

        file << node.id << ",";
        file << (node.isOmega ? "true" : "false") << ",";
        for (double coord : node.coordinates) {
            file << coord << ",";
        }
        file << (node.predecessor ? std::to_string(node.predecessor->id) : "") << ",";
        for (auto it = node.successors.begin(); it != node.successors.end(); ++it) {
            file << (*it)->id;
            if (std::next(it) != node.successors.end()) {
                file << ";";
            }
        }
        file << "\n";
    }
}

int main() {
    int n = 11;
    int ub = 50;
    int d = 2;

    // 随机生成测试点集Ω
    vector<vector<double>> Omega = generateTestSet(n, ub);

    // 计算待选点集N
    vector<vector<double>> N = generateN(Omega, d);
    printN(Omega, N);
     
    // 减少待选点集N的大小
    vector<vector<double>> reducedN = reduceN(N, Omega, d);
    // printReducedN(reducedN);
    N = reducedN;

    // 计算每个点对应的δ(Y)
    map<vector<double>, double> delta = computeDelta(N, Omega);
    // printDelta(delta);

    // 计算每个点的待选前缀，并进行初步筛选
    map<vector<double>, vector<vector<double>>> Pi = computePi(N, delta);
    // printPi(Pi);
    
    // 对点集N进行排序
    sortNodes(N);
    // printSortN(N);

    // 将排序后的点集N转化为Node结构体的向量
    map<vector<double>, int> coordinateToId; // 构建坐标到id的映射
    vector<Node> nodes = convertToNodes(N, coordinateToId);
    printSortedNodes(nodes);

    // 将映射Pi转换为id的待选前缀节点映射并排序
    map<int, vector<int>> idMap = convertPiToIdMap(nodes, Pi, coordinateToId);
    printIdMap(idMap);

    // 获取与Ω中的点对应的节点集合
    vector<Node*> omegaNodes = getOmegaNodes(nodes, Omega, coordinateToId);
    printOmegaNodes(omegaNodes);

    // 获取不属于Ω的N中的点的集合
    vector<Node*> nonOmegaNodes = getNonOmegaNodes(nodes, Omega, coordinateToId);
    printNonOmegaNodes(nonOmegaNodes);

    // 扩展阶段
    double res = expand_reverse(nodes, nodes.size() - 1, idMap, 0);

    // 回溯阶段
    optimalNodes = deepcopy(nodes); //创建最优配置节点集副本
    // printSortedNodes(optimalNodes);
    backtrack(nodes, nodes.size() - 1, idMap, 0);

    // 输出最优 Rectilinear Steiner Arborescence

    // printSteinerArborescence(optimalNodes);
    for (const auto& node : optimalNodes) cout << node.id << ","; cout << endl;
    printTree(optimalNodes, optimalNodes[0]);

    cout << "bestObjective is " << bestObjective << endl;

    saveNodesToCSV(optimalNodes, "RSA_231207_2.csv");

    return 0;
}