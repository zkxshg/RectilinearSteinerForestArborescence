#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>
#include <random>

#include "GraphGenerator_vector.h"

using namespace std;

// 函数声明
//vector<vector<double>> generateN(const vector<vector<double>>&, int);
//vector<double> getGLB(const vector<vector<double>>&);
//vector<vector<vector<double>>> getSubsets(const vector<vector<double>>&, int);


// ================ 生成测试点集  ================
default_random_engine generator;
uniform_real_distribution<double> distribution(0.0);

vector<vector<double>> generateTestSet(int n, int ub) {
    distribution.param(uniform_real_distribution<double>::param_type(0, ub));
    vector<vector<double>> points;
    for (int i = 0; i < n; i++) {
        vector<double> point(2);
        point[0] = distribution(generator);
        point[1] = distribution(generator);
        points.push_back(point);
    }
    return points;
}


//  ================ 基于 Bitmask 遍历Ω不大于d的子集集合  ================
vector<vector<vector<double>>> getSubsets(const vector<vector<double>>& Omega, int size) {
    vector<vector<vector<double>>> subsets;
    int n = Omega.size();

    // Iterate over 2^n possibilities
    for (int i = 1; i < (1 << n); ++i) {
        vector<vector<double>> subset;

        // Check each element if it is in the current subset
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.push_back(Omega[j]);
            }
        }

        // If the subset is of the correct size, add it to the list of subsets
        if ((int)subset.size() <= size) {
            subsets.push_back(subset);
        }
    }

    return subsets;
}

// 测试 getSubsets()结果
void printSubset(const vector<vector<int>>& subset) {
    for (const auto& vec : subset) {
        cout << "(";
        for (int i = 0; i < (int)vec.size(); ++i) {
            cout << vec[i];
            if (i < (int)vec.size() - 1) cout << ", ";
        }
        cout << "), ";
    }
    cout << "\n";
}


//  ================ 生成待选点集 N  ================
//  计算Ω的子集的GLB: T: O(d); d是子集的大小
vector<double> getGLB(const vector<vector<double>>& points) {
    if (points.empty()) {
        throw std::invalid_argument("Point set cannot be empty.");
    }

    // Initialize the GLB as the first point
    double glbX = points[0][0];
    double glbY = points[0][1];

    // Iterate through the point set and update the GLB
    for (const auto& point : points) {
        glbX = min(glbX, point[0]);
        glbY = min(glbY, point[1]);
    }

    return { glbX, glbY };
}

// 基于Ω生成待选点集N: T: O(2^n * (d + m)); S: O(2^n * d + 2^n)
// m是N的当前大小; n是集合Ω的基数
vector<vector<double>> generateN(const vector<vector<double>>& Omega, int d) {
    vector<vector<double>> N;

    // Add {0} to N
    N.push_back(vector<double>{0.0, 0.0});

    // Get all subsets of Omega with size not greater than d
    vector<vector<vector<double>>> subsets = getSubsets(Omega, d);

    // For each subset, compute the GLB and add it to N
    for (const auto& subset : subsets) {
        vector<double> glb = getGLB(subset);
        if (find(N.begin(), N.end(), glb) == N.end()) {
            N.push_back(glb);
        }
    }

    return N;
}

// 打印待选点集N
void printN(const vector<vector<double>>& Omega, const vector<vector<double>>& N) {

    cout << "Omega:" << endl;
    for (const auto& point : Omega) {
        cout << "(" << point[0] << ", " << point[1] << ")" << endl;
    }

    cout << "N:" << endl;
    for (const auto& point : N) {
        cout << "(" << point[0] << ", " << point[1] << ")" << endl;
    }
}


//  ================ 生成待选点集 A  ================
// 偏序关系≤
bool lessThanOrEqual(const vector<double>& p, const vector<double>& q) {
    return p[0] <= q[0] && p[1] <= q[1];
}

// 1-Norm / 曼哈顿距离
double manhattanDistance(const vector<double>& p, const vector<double>& q) {
    return abs(p[0] - q[0]) + abs(p[1] - q[1]);
}

// 计算每个点对应的δ(Y): T: O(n * m); S: O(n)
map<vector<double>, double> computeDelta(const vector<vector<double>>& N, const vector<vector<double>>& Omega) {
    map<vector<double>, double> delta;
    for (const auto& Y : N) {
        double minDistance = numeric_limits<double>::max();
        for (const auto& X : Omega) {
            if (X != Y && lessThanOrEqual(X, Y)) {
                double distance = manhattanDistance(X, Y);
                minDistance = min(minDistance, distance);
            }
        }
        delta[Y] = minDistance;
    }
    return delta;
}

// 打印每个点对应的δ(Y)
void printDelta(const map<vector<double>, double>& delta) {
    cout << "delta:" << endl;
    for (const auto& item : delta) {
        cout << "(" << item.first[0] << ", " << item.first[1] << "): " << item.second << endl;
    }
}

// 计算每个点的待选前缀，并进行初步筛选: O(n^3)
map<vector<double>, vector<vector<double>>> computePi(const vector<vector<double>>& N, const map<vector<double>, double>& delta) {
    map<vector<double>, vector<vector<double>>> Pi;

    // 遍历节点集 N
    for (const auto& Y : N) {
        // 找出待选前缀
        vector<vector<double>> predecessors;

        for (const auto& X : N) {
            // 如果 X != Y 并且 X <= Y，且 X 到 Y 的距离不超过 delta[Y]，则 X 是 Y 的一个待选前缀
            if (X != Y && lessThanOrEqual(X, Y) && manhattanDistance(X, Y) <= delta.at(Y)) {
                predecessors.push_back(X);
            }
        }

        // 对待选前缀进行初步筛选
        vector<vector<double>> filteredPredecessors;
        for (const auto& X : predecessors) {
            bool hasSuccessor = false;

            // 检查 X 是否有后继
            for (const auto& Z : predecessors) {
                // 如果存在一个 Z（Z != X），使得 Z >= X，则 X 有后继
                if (X != Z && lessThanOrEqual(X, Z)) {
                    hasSuccessor = true;
                    break;
                }
            }

            // 如果 X 没有后继，那么它是一个筛选过的前缀
            if (!hasSuccessor) {
                filteredPredecessors.push_back(X);
            }
        }

        // 将筛选过的前缀添加到前缀映射 Pi 中
        Pi[Y] = filteredPredecessors;
    }

    // 返回前缀映射 Pi
    return Pi;
}

// 打印每个点的待选前缀
void printPi(const map<vector<double>, vector<vector<double>>>& Pi) {
    for (const auto& item : Pi) {
        cout << "(" << item.first[0] << ", " << item.first[1] << "):";
        for (const auto& point : item.second) {
            cout << " (" << point[0] << ", " << point[1] << ")";
        }
        cout << endl;
    }
}


// ================ 减少待选点集 N 的大小 ================
vector<vector<double>> reduceN(const vector<vector<double>>& N, const vector<vector<double>>& Omega, int d) {
    vector<vector<double>> reducedN;
    vector<double> zero(d, 0);

    for (const auto& X : N) {
        // cout << X[0] << ":" << X[1] << endl;

        // 如果 X 在 Omega 或 X 是原点，则不进行处理，直接加入 reducedN
        if (find(Omega.begin(), Omega.end(), X) != Omega.end() || X == zero) {
            reducedN.push_back(X);
            continue;
        }
        
        // 对于每个 Y ∈ Ω，检查是否存在满足条件的 Y^(j)
        int count = 0;
        for (const auto& Y : Omega) {
            int equalNum = 0, unequalNum = 0;
            bool match = true;

            for (int j = 0; j < d; ++j) {
                if (Y[j] == X[j]) equalNum++;
                else if (Y[j] < X[j]) {
                    match = false;
                    break;
                }
                else {
                    unequalNum++;
                    if (unequalNum > 1) {
                        match = false;
                        break;
                    }
                }
            }

            if (match && unequalNum == 1) {
                count++;

                if (count > 1) {
                    break;
                }
            }
        }

        // 如果存在超过一个满足条件的 Y^(j)，则 X 加入 reducedN
        if (count > 1) {
            reducedN.push_back(X);
        }
    }

    return reducedN;
}

// 打印减少后的待选点集N
void printReducedN(const vector<vector<double>>& reducedN) {
    cout << "Reduced N:" << endl;
    for (const auto& point : reducedN) {
        cout << "(" << point[0] << ", " << point[1] << ")" << endl;
    }
}

//int main() {
//    int n = 5;
//    int ub = 100;
//    int d = 2;
//
//    // 随机生成测试点集Ω
//    vector<vector<double>> Omega = generateTestSet(n, ub);
//
//    // 计算待选点集N
//    vector<vector<double>> N = generateN(Omega, d);
//    printN(Omega, N);
//
//    // 减少待选点集N的大小
//    vector<vector<double>> reducedN = reduceN(N, Omega, d);
//    // cout << "============= " << reducedN.size() << endl;
//    printReducedN(reducedN);
//    N = reducedN;
//
//    // 计算每个点对应的δ(Y)
//    map<vector<double>, double> delta = computeDelta(N, Omega);
//    printDelta(delta);
//
//    // 计算每个点的待选前缀，并进行初步筛选
//    map<vector<double>, vector<vector<double>>> Pi = computePi(N, delta);
//    printPi(Pi);
//
//    return 0;
//}


