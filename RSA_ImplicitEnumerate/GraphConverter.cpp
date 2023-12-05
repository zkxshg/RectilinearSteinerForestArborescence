#include "GraphGenerator_vector.h"


// ================ 定义排序函数，用于对点集N进行排序 ================
bool compareNodes_old(const vector<double>& node1, const vector<double>& node2) {
    // 按照偏序关系进行比较
    if (lessThanOrEqual(node1, node2)) {
        return true;
    }
    return false;
}

bool compareNodes(const vector<double>& node1, const vector<double>& node2) {
    // 按照 x 坐标进行比较
    if (node1[0] < node2[0]) {
        return true;
    }
    else if (node1[0] > node2[0]) {
        return false;
    }

    // 如果 x 坐标相同，则按照 y 坐标进行比较
    return node1[1] < node2[1];
}

// 对点集N进行排序
void sortNodes(vector<vector<double>>& N) {
    sort(N.begin(), N.end(), compareNodes);
}

// 打印排序后的点集N
void printSortN(const vector<vector<double>>& N) {
    cout << "Sorted N:" << endl;
    for (const auto& point : N) {
        cout << "(" << point[0] << ", " << point[1] << ")" << endl;
    }
}


// ================ 将点集N转化为Node结构 ================
//===  将排序后的点集N转化为Node结构体的向量 ===
vector<Node> convertToNodes(const vector<vector<double>>& N, map<vector<double>, int>& coordinateToId) {
    vector<Node> nodes;
    for (int i = 0; i < N.size(); i++) {
        Node node;
        node.id = i;
        node.isOmega = false;
        node.coordinates = N[i];
        node.predecessor = nullptr;
        nodes.push_back(node);

        // 构建坐标到id的映射
        coordinateToId[N[i]] = i;
    }
    return nodes;
}

// 打印排序后的Node结构体向量
void printSortedNodes(const vector<Node>& nodes) {
    cout << "Sorted Nodes:" << endl;
    for (const Node& node : nodes) {
        cout << "ID: " << node.id << ", Coordinates: (" << node.coordinates[0] << ", " << node.coordinates[1] << ")" << endl;
    }
}


//=== 将映射Pi转换为id的映射 ===
// 计算节点曼哈顿距离
double computeEdgeCost(Node& from, Node& to) {
    double xDist = abs(from.coordinates[0] - to.coordinates[0]);
    double yDist = abs(from.coordinates[1] - to.coordinates[1]);
    return xDist + yDist;
}

// 针对待选前缀按照cost排序
void insertInOrderByCost(vector<int>& ids, int idToInsert, vector<Node>& nodes) {

    for (int i = 0; i < ids.size(); i++) {
        if (i == ids.size() - 1 ||
            computeEdgeCost(nodes[ids[i]], nodes[idToInsert]) <=
            computeEdgeCost(nodes[idToInsert], nodes[ids[i + 1]])) {

            // 在当前位置插入
            ids.insert(ids.begin() + i, idToInsert);
            return;
        }
    }

    // 插入到向量末尾
    ids.push_back(idToInsert);
}

// 将Pi转为idMap并按照cost排序
map<int, vector<int>> convertPiToIdMap(vector<Node>& nodes, const map<vector<double>, vector<vector<double>>>& Pi, const map<vector<double>, int>& coordinateToId) {
    map<int, vector<int>> idMap;

    for (const auto& item : Pi) {
        int nodeId = coordinateToId.at(item.first);
        vector<int> prefixIds;

        for (const auto& prefix : item.second) {
            int prefixId = coordinateToId.at(prefix);
            // prefixIds.push_back(prefixId);
            // 按代价插入排序
            insertInOrderByCost(prefixIds, prefixId, nodes);
        }
        idMap[nodeId] = prefixIds;
    }
    return idMap;
}

// 打印id映射的结果
void printIdMap(const map<int, vector<int>>& idMap) {
    cout << "ID Map:" << endl;
    for (const auto& item : idMap) {
        cout << "ID: " << item.first << ", Prefixes:";
        for (int prefixId : item.second) {
            cout << " " << prefixId;
        }
        cout << endl;
    }
}

// === 获取与Ω中的点对应的节点集合 ===
vector<Node*> getOmegaNodes(vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId) {
    vector<Node*> omegaNodes;
    for (const auto& point : Omega) {
        if (coordinateToId.count(point) > 0) {
            nodes[coordinateToId[point]].isOmega = true;
            omegaNodes.push_back(const_cast<Node*>(&nodes[coordinateToId[point]]));
        }
    }
    return omegaNodes;
}

// 打印omegaNodes中节点的id和对应的坐标
void printOmegaNodes(const vector<Node*>& omegaNodes) {
    cout << "Omega Nodes:" << endl;
    for (const auto& node : omegaNodes) {
        cout << "ID: " << node->id << ", isOmega: " << node->isOmega << 
            ", Coordinates: (" << node->coordinates[0] << ", " << node->coordinates[1] << ")" << endl;
    }
}

// === 获取不属于Ω的N中的点的集合 ===
vector<Node*> getNonOmegaNodes(const vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId) {
    vector<Node*> nonOmegaNodes;
    for (const auto& node : nodes) {
        if (coordinateToId.count(node.coordinates) > 0 && find(Omega.begin(), Omega.end(), node.coordinates) == Omega.end()) {
            nonOmegaNodes.push_back(const_cast<Node*>(&node));
        }
    }
    return nonOmegaNodes;
}

// 打印非Ω节点的id和对应的坐标
void printNonOmegaNodes(const vector<Node*>& nonOmegaNodes) {
    cout << "Non-Omega Nodes:" << endl;
    for (const auto& node : nonOmegaNodes) {
        cout << "ID: " << node->id << ", isOmega: " << node->isOmega << 
            ", Coordinates: (" << node->coordinates[0] << ", " << node->coordinates[1] << ")" << endl;
    }
}

// 复制节点集nodes
vector<Node> deepcopy(const vector<Node>& nodes) {
    unordered_map<const Node*, Node*> map;
    vector<Node> newNodes;

    // 创建新节点并复制原节点的值，但不包括指针
    for (const Node& node : nodes) {
        Node* newNode = new Node;
        newNode->id = node.id;
        newNode->isOmega = node.isOmega;
        newNode->coordinates = node.coordinates;
        // 先设置为nullptr，稍后再处理
        newNode->predecessor = nullptr;

        map[&node] = newNode;
        newNodes.push_back(*newNode);
    }

    // 现在处理指针
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i].predecessor != nullptr) {
            newNodes[i].predecessor = map[nodes[i].predecessor];
        }
        for (const Node* successor : nodes[i].successors) {
            newNodes[i].successors.insert(map[successor]);
        }
    }

    return newNodes;
}

// 复制前后缀关系
void copyRelationships(const vector<Node>& nodes, vector<Node>& newNodes) {
    unordered_map<int, Node*> map;

    // 创建一个从id到节点的映射
    for (Node& node : newNodes) {
        map[node.id] = &node;
    }

    // 遍历nodes，复制前后缀关系到newNodes
    for (const Node& node : nodes) {
        Node* newNode = map[node.id];
        if (newNode != nullptr) {
            if (node.predecessor != nullptr) {
                // 找到新的前驱节点
                newNode->predecessor = map[node.predecessor->id];
            }
            newNode->successors.clear();
            for (const Node* successor : node.successors) {
                // 找到新的后继节点
                newNode->successors.insert(map[successor->id]);
            }
        }
    }
}