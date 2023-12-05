#include "GraphGenerator_vector.h"

// ================ ������һ�滮 ================
double bestObjective = INT_MAX;
map<int, int> prefixIndex;
vector<Node> optimalNodes;

// ���ڵ㼯�����Ƿ����node
bool containsNode(const vector<Node*>& omegaNodes, Node* node) {
    for (const auto& n : omegaNodes) {
        if (n == node) {
            return true;
        }
    }
    return false;
}

// �������ǰ׺�ķǦ��ڵ�
void cleanNodesInfeasible(vector<Node>& nodes) {
    for (Node& node : nodes) {
        if (!node.isOmega) {
            // ���һ���ڵ�ֻ��ǰ���ڵ㵫û�к�̽ڵ㣬���������ǰ���ڵ㲢������̽ڵ��б�
            if (node.predecessor && node.successors.empty()) {
                node.predecessor = nullptr;
                node.successors.clear();
            }
        }
    }
}

// ����ڵ㼯nodes��objective function: min ��_(i,j)��A c_ij x_ij  (4.1)
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
                // �Ǧ���ֻ�����û�г���,���ǰ׺�ͳ���
                node->predecessor = nullptr;
                node->successors.clear();

                cout << "Clear Node" << node->id << endl;

                continue;
            }
        }

        // ���node�ıߵ�cost
        for (Node* successor : node->successors) {
            cost += computeEdgeCost(*node, *successor);
        }

    }

    return cost;
}

// === ���Լ������һ�Ƿ�����: ��_h��P_i x_hi �� 1, i �� ��, i �� 0  (4.2) ===
bool checkConstraintOne(const vector<Node*>& omegaNodes) {
    for (const auto& node : omegaNodes) {
        if (node->predecessor == nullptr) {
            return false;
        }
    }
    return true;
}

// ������ڦ��ĵ���
bool checkConstraintOneSingle(Node* node) {
    if (node->predecessor == nullptr) {
        return false;
    }
    return true;
}

// === ���Լ���������Ƿ�����: |S_i|��_h��P_i x_hi - ��_j��S_i x_ij �� 0, i �� N - ��, i �� 0  (4.3) ===
bool checkConstraintTwo(const vector<Node*>& nodes) {
    for (const auto& node : nodes) {
        if (node->successors.empty()) {
            // �޳��ߣ�������Ϊ0
            if (node->predecessor != nullptr) {
                return false;
            }
        }
        else {
            // �г��ߣ�������Ϊ1
            if (node->predecessor == nullptr) {
                return false;
            }
        }
    }
    return true;
}

// ������� N - ���ĵ���
bool checkConstraintTwoSingle(Node* node) {
    if (node->successors.empty()) {
        // �޳��ߣ�������Ϊ0
        if (node->predecessor != nullptr) {
            return false;
        }
    }
    else {
        // �г��ߣ�������Ϊ1
        if (node->predecessor == nullptr) {
            return false;
        }
    }
    return true;
}


// ================ ��ʽ�оٷ������һ�滮 ================
// ѭ�����ǰ׺�Ƿ���в��ڼ����±�ʱ����Ŀ��ֵ
double checkConstraintTwoLoop(Node* node, map<int, vector<int>>& idMap, vector<Node>& nodes, double currentObj) {
    double obj = currentObj; // ʹ�ô����Ŀ�꺯��ֵ

    while (!checkConstraintTwoSingle(node) && node->id != 0) {
        // �ҵ���Сcost�������Ϊǰ��
        int minCostPrefixId = idMap[node->id][prefixIndex[node->id]];
        Node* minCostPrefixNode = &nodes[minCostPrefixId];
        node->predecessor = minCostPrefixNode;

        // ����ǰ�ڵ���ӵ�ǰ���ڵ�ĺ�̽ڵ��б���
        minCostPrefixNode->successors.insert(node);

        // �����±ߵ�cost���ӵ�Ŀ�꺯��ֵ��
        obj += computeEdgeCost(*node, *minCostPrefixNode);

        // ���µ�ǰ�ڵ�Ϊǰ���ڵ㣬�������Լ������
        node = minCostPrefixNode;
    }

    // �����޸ĺ��Ŀ�꺯��ֵ
    return obj;
}

// === ��չ�׶� ===
void expand(vector<Node>& nodes, int k, map<int, vector<int>>& idMap, double currentObj) {
    Node* node = &nodes[k];

    // ���Լ����������Բ���������ǰ���ڵ�
    if (node->isOmega) {
        // node���ڦ������Լ������һ
        if (!checkConstraintOneSingle(node)) {
            // ����Сcost�������Ϊǰ�� 
            int minCostPrefixId = idMap[node->id][prefixIndex[node->id]];
            Node* minCostPrefixNode = &nodes[minCostPrefixId];
            node->predecessor = minCostPrefixNode;

            // ����ǰ�ڵ���ӵ�ǰ���ڵ�ĺ�̽ڵ��б���
            minCostPrefixNode->successors.insert(node);

            // �����±ߵ�cost���ӵ�Ŀ�꺯��ֵ��
            currentObj += computeEdgeCost(*node, *minCostPrefixNode);

            // ���Լ��������
            node = minCostPrefixNode;
            currentObj = checkConstraintTwoLoop(node, idMap, nodes, currentObj);
        }
    }
    else {
        // ���Լ��������
        currentObj = checkConstraintTwoLoop(node, idMap, nodes, currentObj);
    }

    // ���Ŀ�꺯����ֵ������ǰ���Ž⣬��������
    if (currentObj > bestObjective) {
        return;
    }

    // �������δ����Ľڵ㣬������չ
    if (k < int(nodes.size()) - 1) {
        expand(nodes, k + 1, idMap, currentObj);
    }
    else {
        // �������Ž�
        bestObjective = computeObjectiveFunction(nodes);
        cout << bestObjective << ":" << currentObj << endl;
    }
}

// ��չ�׶�-����
double expand_reverse(vector<Node>& nodes, int k, map<int, vector<int>>& idMap, double currentObj) {
    Node* node = &nodes[k];

    if (node->isOmega && k != 0) {
        // ���Լ������һ
        if (!checkConstraintOneSingle(node)) {
            // ����Сcost�������Ϊǰ��
            int minCostPrefixId = idMap[node->id][prefixIndex[node->id]];
            Node* minCostPrefixNode = &nodes[minCostPrefixId];
            // ���µ�ǰ�ڵ��ǰ׺
            node->predecessor = minCostPrefixNode;
            // ����ǰ�ڵ����ǰ׺�ڵ�ĺ�׺����
            minCostPrefixNode->successors.insert(node);
            //  ���� ObjectiveValue
            currentObj += computeEdgeCost(*node, *minCostPrefixNode);
        }
    }
    else if (k > 0) {
        // ���Լ��������
        if (!checkConstraintTwoSingle(node)) {
            // ����Сcost�������Ϊǰ��
            int minCostPrefixId = idMap[node->id][prefixIndex[node->id]];
            Node* minCostPrefixNode = &nodes[minCostPrefixId];
            // ���µ�ǰ�ڵ��ǰ׺
            node->predecessor = minCostPrefixNode;
            // ����ǰ�ڵ����ǰ׺�ڵ�ĺ�׺����
            minCostPrefixNode->successors.insert(node);
            //  ���� ObjectiveValue
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
        // ����k = 0�������ֻ�������Ž�
        if (bestObjective > currentObj) {
            cout << bestObjective << ":" << currentObj << endl;
            copyRelationships(nodes, optimalNodes);  // �������Žڵ�����

        }
        bestObjective = min(bestObjective, currentObj);
    }

    return currentObj;
}

// === ���ݽ׶� ===

// ���� (0, end] �����нڵ��໥��ǰ��׺
void clearPredecessor(int end, vector<Node>& nodes) {
    for (int i = end; i > 0; i--) {
        // �����ǰ�ڵ��ǰ׺�Ͷ�Ӧǰ׺�ڵ�ĺ�׺
        if (nodes[i].predecessor != nullptr) {
            nodes[i].predecessor->successors.erase(&nodes[i]);
            nodes[i].predecessor = nullptr;
        }

        // ���õ�ǰǰ׺����
        prefixIndex[i] = 0;
    }

}

// ���еݹ����
void backtrack(vector<Node>& nodes, int l, map<int, vector<int>>& idMap, double currentObj) {
    // cout << "Backtrack " << l << ":" << endl;

    if (l > 0) {
        vector<int> prefixes = idMap[l];
        // int cou = 0;  cout << "===========" << cou++ << "===========" << endl;
        for (int p : prefixes) {
            // �����l֮ǰ�����нڵ�֮��ı�
            clearPredecessor(l - 1, nodes);

            // ����ǰ���Ͷ�Ӧ�ڵ�ĺ�׺
            nodes[l].predecessor = &nodes[p];
            nodes[p].successors.insert(&nodes[l]);

            // ����ObjCost
            double preObj = currentObj + computeEdgeCost(nodes[l], nodes[p]);

            if (preObj < bestObjective) {
                // ����objCost
                double obj = expand_reverse(nodes, l - 1, idMap, preObj);

                // �ݹ���չ
                backtrack(nodes, l - 1, idMap, preObj);
            }

            // ����ǰ�� 
            nodes[p].successors.erase(&nodes[l]);
            nodes[l].predecessor = nullptr;
        }

    }

}

// ���������RSA
void printSteinerArborescence(const vector<Node>& nodes) {
    for (const auto& node : nodes) {
        // ����ڵ����Ϣ
        cout << "Node ID: " << node.id << endl;
        cout << "Coordinates: (" << node.coordinates[0] << ", " << node.coordinates[1] << ")" << endl;

        // ���ǰ���ڵ����Ϣ
        if (node.predecessor != nullptr) {
            cout << "Predecessor ID: " << node.predecessor->id << endl;
            cout << "Predecessor Coordinates: (" << node.predecessor->coordinates[0] << ", " << node.predecessor->coordinates[1] << ")" << endl;
        }

        // �����̽ڵ����Ϣ
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
    // �����ǰ�ڵ����Ϣ���㼶
    cout << string(level, '\t') << "Node ID: " << currentNode.id; if (currentNode.isOmega) cout << "*"; cout << endl;
    cout << string(level, '\t') << "Coordinates: (" << currentNode.coordinates[0] << ", " << currentNode.coordinates[1] << ")" << endl;

    // �����ǰ�ڵ�ĺ�̽ڵ����Ϣ
    cout << string(level, '\t') << "Successor IDs:";
    for (const auto& successor : currentNode.successors) {
        cout << " " << successor->id;
    }
    cout << endl;

    // �ݹ������̽ڵ������
    for (const auto& successor : currentNode.successors) {
        printTree(nodes, *successor, level + 1);
    }
}

// ���ṹд��csv�ļ�
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
    int n = 9;
    int ub = 100;
    int d = 2;

    // ������ɲ��Ե㼯��
    vector<vector<double>> Omega = generateTestSet(n, ub);

    // �����ѡ�㼯N
    vector<vector<double>> N = generateN(Omega, d);
    printN(Omega, N);
     
    // ���ٴ�ѡ�㼯N�Ĵ�С
    vector<vector<double>> reducedN = reduceN(N, Omega, d);
    // printReducedN(reducedN);
    N = reducedN;

    // ����ÿ�����Ӧ�Ħ�(Y)
    map<vector<double>, double> delta = computeDelta(N, Omega);
    // printDelta(delta);

    // ����ÿ����Ĵ�ѡǰ׺�������г���ɸѡ
    map<vector<double>, vector<vector<double>>> Pi = computePi(N, delta);
    // printPi(Pi);
    
    // �Ե㼯N��������
    sortNodes(N);
    // printSortN(N);

    // �������ĵ㼯Nת��ΪNode�ṹ�������
    map<vector<double>, int> coordinateToId; // �������굽id��ӳ��
    vector<Node> nodes = convertToNodes(N, coordinateToId);
    printSortedNodes(nodes);

    // ��ӳ��Piת��Ϊid�Ĵ�ѡǰ׺�ڵ�ӳ�䲢����
    map<int, vector<int>> idMap = convertPiToIdMap(nodes, Pi, coordinateToId);
    printIdMap(idMap);

    // ��ȡ�릸�еĵ��Ӧ�Ľڵ㼯��
    vector<Node*> omegaNodes = getOmegaNodes(nodes, Omega, coordinateToId);
    printOmegaNodes(omegaNodes);

    // ��ȡ�����ڦ���N�еĵ�ļ���
    vector<Node*> nonOmegaNodes = getNonOmegaNodes(nodes, Omega, coordinateToId);
    printNonOmegaNodes(nonOmegaNodes);

    // ��չ�׶�
    double res = expand_reverse(nodes, nodes.size() - 1, idMap, 0);

    // ���ݽ׶�
    optimalNodes = deepcopy(nodes); //�����������ýڵ㼯����
    // printSortedNodes(optimalNodes);
    backtrack(nodes, nodes.size() - 1, idMap, 0);

    // ������� Rectilinear Steiner Arborescence

    // printSteinerArborescence(optimalNodes);
    for (const auto& node : optimalNodes) cout << node.id << ","; cout << endl;
    printTree(optimalNodes, optimalNodes[0]);

    cout << "bestObjective is " << bestObjective << endl;

    saveNodesToCSV(optimalNodes, "RSA_231205.csv");

    return 0;
}