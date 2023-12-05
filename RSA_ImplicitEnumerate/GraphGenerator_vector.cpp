#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>
#include <random>

#include "GraphGenerator_vector.h"

using namespace std;

// ��������
//vector<vector<double>> generateN(const vector<vector<double>>&, int);
//vector<double> getGLB(const vector<vector<double>>&);
//vector<vector<vector<double>>> getSubsets(const vector<vector<double>>&, int);


// ================ ���ɲ��Ե㼯  ================
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


//  ================ ���� Bitmask ������������d���Ӽ�����  ================
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

// ���� getSubsets()���
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


//  ================ ���ɴ�ѡ�㼯 N  ================
//  ���㦸���Ӽ���GLB: T: O(d); d���Ӽ��Ĵ�С
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

// ���ڦ����ɴ�ѡ�㼯N: T: O(2^n * (d + m)); S: O(2^n * d + 2^n)
// m��N�ĵ�ǰ��С; n�Ǽ��Ϧ��Ļ���
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

// ��ӡ��ѡ�㼯N
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


//  ================ ���ɴ�ѡ�㼯 A  ================
// ƫ���ϵ��
bool lessThanOrEqual(const vector<double>& p, const vector<double>& q) {
    return p[0] <= q[0] && p[1] <= q[1];
}

// 1-Norm / �����پ���
double manhattanDistance(const vector<double>& p, const vector<double>& q) {
    return abs(p[0] - q[0]) + abs(p[1] - q[1]);
}

// ����ÿ�����Ӧ�Ħ�(Y): T: O(n * m); S: O(n)
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

// ��ӡÿ�����Ӧ�Ħ�(Y)
void printDelta(const map<vector<double>, double>& delta) {
    cout << "delta:" << endl;
    for (const auto& item : delta) {
        cout << "(" << item.first[0] << ", " << item.first[1] << "): " << item.second << endl;
    }
}

// ����ÿ����Ĵ�ѡǰ׺�������г���ɸѡ: O(n^3)
map<vector<double>, vector<vector<double>>> computePi(const vector<vector<double>>& N, const map<vector<double>, double>& delta) {
    map<vector<double>, vector<vector<double>>> Pi;

    // �����ڵ㼯 N
    for (const auto& Y : N) {
        // �ҳ���ѡǰ׺
        vector<vector<double>> predecessors;

        for (const auto& X : N) {
            // ��� X != Y ���� X <= Y���� X �� Y �ľ��벻���� delta[Y]���� X �� Y ��һ����ѡǰ׺
            if (X != Y && lessThanOrEqual(X, Y) && manhattanDistance(X, Y) <= delta.at(Y)) {
                predecessors.push_back(X);
            }
        }

        // �Դ�ѡǰ׺���г���ɸѡ
        vector<vector<double>> filteredPredecessors;
        for (const auto& X : predecessors) {
            bool hasSuccessor = false;

            // ��� X �Ƿ��к��
            for (const auto& Z : predecessors) {
                // �������һ�� Z��Z != X����ʹ�� Z >= X���� X �к��
                if (X != Z && lessThanOrEqual(X, Z)) {
                    hasSuccessor = true;
                    break;
                }
            }

            // ��� X û�к�̣���ô����һ��ɸѡ����ǰ׺
            if (!hasSuccessor) {
                filteredPredecessors.push_back(X);
            }
        }

        // ��ɸѡ����ǰ׺��ӵ�ǰ׺ӳ�� Pi ��
        Pi[Y] = filteredPredecessors;
    }

    // ����ǰ׺ӳ�� Pi
    return Pi;
}

// ��ӡÿ����Ĵ�ѡǰ׺
void printPi(const map<vector<double>, vector<vector<double>>>& Pi) {
    for (const auto& item : Pi) {
        cout << "(" << item.first[0] << ", " << item.first[1] << "):";
        for (const auto& point : item.second) {
            cout << " (" << point[0] << ", " << point[1] << ")";
        }
        cout << endl;
    }
}


// ================ ���ٴ�ѡ�㼯 N �Ĵ�С ================
vector<vector<double>> reduceN(const vector<vector<double>>& N, const vector<vector<double>>& Omega, int d) {
    vector<vector<double>> reducedN;
    vector<double> zero(d, 0);

    for (const auto& X : N) {
        // cout << X[0] << ":" << X[1] << endl;

        // ��� X �� Omega �� X ��ԭ�㣬�򲻽��д���ֱ�Ӽ��� reducedN
        if (find(Omega.begin(), Omega.end(), X) != Omega.end() || X == zero) {
            reducedN.push_back(X);
            continue;
        }
        
        // ����ÿ�� Y �� ��������Ƿ�������������� Y^(j)
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

        // ������ڳ���һ������������ Y^(j)���� X ���� reducedN
        if (count > 1) {
            reducedN.push_back(X);
        }
    }

    return reducedN;
}

// ��ӡ���ٺ�Ĵ�ѡ�㼯N
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
//    // ������ɲ��Ե㼯��
//    vector<vector<double>> Omega = generateTestSet(n, ub);
//
//    // �����ѡ�㼯N
//    vector<vector<double>> N = generateN(Omega, d);
//    printN(Omega, N);
//
//    // ���ٴ�ѡ�㼯N�Ĵ�С
//    vector<vector<double>> reducedN = reduceN(N, Omega, d);
//    // cout << "============= " << reducedN.size() << endl;
//    printReducedN(reducedN);
//    N = reducedN;
//
//    // ����ÿ�����Ӧ�Ħ�(Y)
//    map<vector<double>, double> delta = computeDelta(N, Omega);
//    printDelta(delta);
//
//    // ����ÿ����Ĵ�ѡǰ׺�������г���ɸѡ
//    map<vector<double>, vector<vector<double>>> Pi = computePi(N, delta);
//    printPi(Pi);
//
//    return 0;
//}


