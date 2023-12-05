#include "GraphGenerator_vector.h"


// ================ ���������������ڶԵ㼯N�������� ================
bool compareNodes_old(const vector<double>& node1, const vector<double>& node2) {
    // ����ƫ���ϵ���бȽ�
    if (lessThanOrEqual(node1, node2)) {
        return true;
    }
    return false;
}

bool compareNodes(const vector<double>& node1, const vector<double>& node2) {
    // ���� x ������бȽ�
    if (node1[0] < node2[0]) {
        return true;
    }
    else if (node1[0] > node2[0]) {
        return false;
    }

    // ��� x ������ͬ������ y ������бȽ�
    return node1[1] < node2[1];
}

// �Ե㼯N��������
void sortNodes(vector<vector<double>>& N) {
    sort(N.begin(), N.end(), compareNodes);
}

// ��ӡ�����ĵ㼯N
void printSortN(const vector<vector<double>>& N) {
    cout << "Sorted N:" << endl;
    for (const auto& point : N) {
        cout << "(" << point[0] << ", " << point[1] << ")" << endl;
    }
}


// ================ ���㼯Nת��ΪNode�ṹ ================
//===  �������ĵ㼯Nת��ΪNode�ṹ������� ===
vector<Node> convertToNodes(const vector<vector<double>>& N, map<vector<double>, int>& coordinateToId) {
    vector<Node> nodes;
    for (int i = 0; i < N.size(); i++) {
        Node node;
        node.id = i;
        node.isOmega = false;
        node.coordinates = N[i];
        node.predecessor = nullptr;
        nodes.push_back(node);

        // �������굽id��ӳ��
        coordinateToId[N[i]] = i;
    }
    return nodes;
}

// ��ӡ������Node�ṹ������
void printSortedNodes(const vector<Node>& nodes) {
    cout << "Sorted Nodes:" << endl;
    for (const Node& node : nodes) {
        cout << "ID: " << node.id << ", Coordinates: (" << node.coordinates[0] << ", " << node.coordinates[1] << ")" << endl;
    }
}


//=== ��ӳ��Piת��Ϊid��ӳ�� ===
// ����ڵ������پ���
double computeEdgeCost(Node& from, Node& to) {
    double xDist = abs(from.coordinates[0] - to.coordinates[0]);
    double yDist = abs(from.coordinates[1] - to.coordinates[1]);
    return xDist + yDist;
}

// ��Դ�ѡǰ׺����cost����
void insertInOrderByCost(vector<int>& ids, int idToInsert, vector<Node>& nodes) {

    for (int i = 0; i < ids.size(); i++) {
        if (i == ids.size() - 1 ||
            computeEdgeCost(nodes[ids[i]], nodes[idToInsert]) <=
            computeEdgeCost(nodes[idToInsert], nodes[ids[i + 1]])) {

            // �ڵ�ǰλ�ò���
            ids.insert(ids.begin() + i, idToInsert);
            return;
        }
    }

    // ���뵽����ĩβ
    ids.push_back(idToInsert);
}

// ��PiתΪidMap������cost����
map<int, vector<int>> convertPiToIdMap(vector<Node>& nodes, const map<vector<double>, vector<vector<double>>>& Pi, const map<vector<double>, int>& coordinateToId) {
    map<int, vector<int>> idMap;

    for (const auto& item : Pi) {
        int nodeId = coordinateToId.at(item.first);
        vector<int> prefixIds;

        for (const auto& prefix : item.second) {
            int prefixId = coordinateToId.at(prefix);
            // prefixIds.push_back(prefixId);
            // �����۲�������
            insertInOrderByCost(prefixIds, prefixId, nodes);
        }
        idMap[nodeId] = prefixIds;
    }
    return idMap;
}

// ��ӡidӳ��Ľ��
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

// === ��ȡ�릸�еĵ��Ӧ�Ľڵ㼯�� ===
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

// ��ӡomegaNodes�нڵ��id�Ͷ�Ӧ������
void printOmegaNodes(const vector<Node*>& omegaNodes) {
    cout << "Omega Nodes:" << endl;
    for (const auto& node : omegaNodes) {
        cout << "ID: " << node->id << ", isOmega: " << node->isOmega << 
            ", Coordinates: (" << node->coordinates[0] << ", " << node->coordinates[1] << ")" << endl;
    }
}

// === ��ȡ�����ڦ���N�еĵ�ļ��� ===
vector<Node*> getNonOmegaNodes(const vector<Node>& nodes, const vector<vector<double>>& Omega, map<vector<double>, int>& coordinateToId) {
    vector<Node*> nonOmegaNodes;
    for (const auto& node : nodes) {
        if (coordinateToId.count(node.coordinates) > 0 && find(Omega.begin(), Omega.end(), node.coordinates) == Omega.end()) {
            nonOmegaNodes.push_back(const_cast<Node*>(&node));
        }
    }
    return nonOmegaNodes;
}

// ��ӡ�Ǧ��ڵ��id�Ͷ�Ӧ������
void printNonOmegaNodes(const vector<Node*>& nonOmegaNodes) {
    cout << "Non-Omega Nodes:" << endl;
    for (const auto& node : nonOmegaNodes) {
        cout << "ID: " << node->id << ", isOmega: " << node->isOmega << 
            ", Coordinates: (" << node->coordinates[0] << ", " << node->coordinates[1] << ")" << endl;
    }
}

// ���ƽڵ㼯nodes
vector<Node> deepcopy(const vector<Node>& nodes) {
    unordered_map<const Node*, Node*> map;
    vector<Node> newNodes;

    // �����½ڵ㲢����ԭ�ڵ��ֵ����������ָ��
    for (const Node& node : nodes) {
        Node* newNode = new Node;
        newNode->id = node.id;
        newNode->isOmega = node.isOmega;
        newNode->coordinates = node.coordinates;
        // ������Ϊnullptr���Ժ��ٴ���
        newNode->predecessor = nullptr;

        map[&node] = newNode;
        newNodes.push_back(*newNode);
    }

    // ���ڴ���ָ��
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

// ����ǰ��׺��ϵ
void copyRelationships(const vector<Node>& nodes, vector<Node>& newNodes) {
    unordered_map<int, Node*> map;

    // ����һ����id���ڵ��ӳ��
    for (Node& node : newNodes) {
        map[node.id] = &node;
    }

    // ����nodes������ǰ��׺��ϵ��newNodes
    for (const Node& node : nodes) {
        Node* newNode = map[node.id];
        if (newNode != nullptr) {
            if (node.predecessor != nullptr) {
                // �ҵ��µ�ǰ���ڵ�
                newNode->predecessor = map[node.predecessor->id];
            }
            newNode->successors.clear();
            for (const Node* successor : node.successors) {
                // �ҵ��µĺ�̽ڵ�
                newNode->successors.insert(map[successor->id]);
            }
        }
    }
}