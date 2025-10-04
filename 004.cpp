#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// 表示代数表达式的类
class AlgebraicExpression {
private:
    map<string, double> terms; // 项系数，key为变量组合如 "x", "xy", "1"(常数项)

public:
    AlgebraicExpression() {}

    // 构造函数：常数
    AlgebraicExpression(double constant) {
        if (constant != 0) {
            terms["1"] = constant;
        }
    }

    // 构造函数：单个变量
    AlgebraicExpression(const string& variable) {
        terms[variable] = 1.0;
    }

    // 加法运算符重载
    AlgebraicExpression operator+(const AlgebraicExpression& other) const {
        AlgebraicExpression result = *this;
        for (const auto& term : other.terms) {
            result.terms[term.first] += term.second;
            if (result.terms[term.first] == 0) {
                result.terms.erase(term.first);
            }
        }
        return result;
    }

    // 减法运算符重载
    AlgebraicExpression operator-(const AlgebraicExpression& other) const {
        AlgebraicExpression result = *this;
        for (const auto& term : other.terms) {
            result.terms[term.first] -= term.second;
            if (result.terms[term.first] == 0) {
                result.terms.erase(term.first);
            }
        }
        return result;
    }

    // 乘法运算符重载
    AlgebraicExpression operator*(const AlgebraicExpression& other) const {
        AlgebraicExpression result;

        for (const auto& term1 : terms) {
            for (const auto& term2 : other.terms) {
                string newKey = multiplyVariables(term1.first, term2.first);
                double newCoeff = term1.second * term2.second;

                result.terms[newKey] += newCoeff;
                if (result.terms[newKey] == 0) {
                    result.terms.erase(newKey);
                }
            }
        }

        return result;
    }

    // 乘以常数
    AlgebraicExpression operator*(double scalar) const {
        AlgebraicExpression result;
        for (const auto& term : terms) {
            result.terms[term.first] = term.second * scalar;
        }
        return result;
    }

    // 友元函数：常数乘以表达式
    friend AlgebraicExpression operator*(double scalar, const AlgebraicExpression& expr) {
        return expr * scalar;
    }

    // 判断是否为0
    bool isZero() const {
        return terms.empty();
    }

    // 转换为字符串
    string toString() const {
        if (terms.empty()) {
            return "0";
        }

        stringstream ss;
        bool firstTerm = true;

        for (const auto& term : terms) {
            if (!firstTerm) {
                if (term.second > 0) {
                    ss << " + ";
                }
                else {
                    ss << " - ";
                }
            }
            else {
                if (term.second < 0) {
                    ss << "-";
                }
            }

            double absCoeff = abs(term.second);

            if (term.first == "1") {
                // 常数项
                if (firstTerm && term.second < 0) {
                    ss << absCoeff;
                }
                else {
                    ss << (firstTerm ? "" : (absCoeff == 1 ? "" : to_string(absCoeff)));
                    if (absCoeff != 1 || firstTerm) {
                        ss << absCoeff;
                    }
                }
            }
            else {
                // 变量项
                if (absCoeff != 1 || firstTerm) {
                    ss << absCoeff;
                }
                ss << term.first;
            }

            firstTerm = false;
        }

        return ss.str();
    }

private:
    // 辅助函数：乘变量名（排序和合并）
    string multiplyVariables(const string& var1, const string& var2) const {
        if (var1 == "1") return var2;
        if (var2 == "1") return var1;

        // 分割变量并排序
        vector<string> vars;
        if (!var1.empty()) vars.push_back(var1);
        if (!var2.empty()) vars.push_back(var2);

        // 简单实现：这里假设变量都是单字符
        string result;
        for (const auto& v : vars) {
            result += v;
        }

        // 对字符排序（可选，使输出更规范）
        sort(result.begin(), result.end());

        return result;
    }
};

// 函数声明
void printMatrix(const vector<vector<AlgebraicExpression>>& matrix);
AlgebraicExpression calculateDeterminant(vector<vector<AlgebraicExpression>> matrix);
vector<vector<AlgebraicExpression>> getSubMatrix(const vector<vector<AlgebraicExpression>>& matrix, int row, int col);
AlgebraicExpression parseExpression(const string& input);

int main() {
    int n;

    cout << "=== 符号矩阵行列式计算器 ===" << endl;
    cout << "支持输入数字和变量(x,y,z等)" << endl;
    cout << "请输入矩阵的阶数: ";
    cin >> n;

    if (n <= 0) {
        cout << "错误：矩阵阶数必须为正整数！" << endl;
        return 1;
    }

    // 创建n×n矩阵
    vector<vector<AlgebraicExpression>> matrix(n, vector<AlgebraicExpression>(n));

    cout << "\n请输入矩阵元素（按行输入，可用x,y,z等变量）:" << endl;
    cin.ignore(); // 清除输入缓冲区

    for (int i = 0; i < n; i++) {
        cout << "第 " << i + 1 << " 行: ";
        string line;
        getline(cin, line);

        stringstream ss(line);
        for (int j = 0; j < n; j++) {
            string element;
            ss >> element;
            matrix[i][j] = parseExpression(element);
        }
    }

    // 显示输入的矩阵
    cout << "\n您输入的矩阵为:" << endl;
    printMatrix(matrix);

    // 计算行列式
    cout << "\n计算行列式..." << endl;
    AlgebraicExpression determinant = calculateDeterminant(matrix);

    // 输出结果
    cout << "\n行列式表达式: " << determinant.toString() << endl;

    return 0;
}

// 解析输入字符串为代数表达式
AlgebraicExpression parseExpression(const string& input) {
    // 检查是否为纯数字
    bool isNumber = true;
    for (char c : input) {
        if (!isdigit(c) && c != '.' && c != '-') {
            isNumber = false;
            break;
        }
    }

    if (isNumber) {
        return AlgebraicExpression(stod(input));
    }
    else {
        // 假设输入是单个变量
        return AlgebraicExpression(input);
    }
}

// 打印矩阵
void printMatrix(const vector<vector<AlgebraicExpression>>& matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        cout << "| ";
        for (int j = 0; j < n; j++) {
            cout << setw(12) << matrix[i][j].toString() << " ";
        }
        cout << "|" << endl;
    }
}

// 获取子矩阵（去掉第row行和第col列）
vector<vector<AlgebraicExpression>> getSubMatrix(const vector<vector<AlgebraicExpression>>& matrix, int row, int col) {
    int n = matrix.size();
    vector<vector<AlgebraicExpression>> subMatrix(n - 1, vector<AlgebraicExpression>(n - 1));

    int sub_i = 0;
    for (int i = 0; i < n; i++) {
        if (i == row) continue;

        int sub_j = 0;
        for (int j = 0; j < n; j++) {
            if (j == col) continue;

            subMatrix[sub_i][sub_j] = matrix[i][j];
            sub_j++;
        }
        sub_i++;
    }

    return subMatrix;
}

// 递归计算行列式
AlgebraicExpression calculateDeterminant(vector<vector<AlgebraicExpression>> matrix) {
    int n = matrix.size();

    // 1×1矩阵的情况
    if (n == 1) {
        return matrix[0][0];
    }

    // 2×2矩阵的情况
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    // n×n矩阵（n>=3）：使用拉普拉斯展开沿第一行展开
    AlgebraicExpression determinant;

    for (int j = 0; j < n; j++) {
        // 计算代数余子式
        vector<vector<AlgebraicExpression>> subMatrix = getSubMatrix(matrix, 0, j);
        AlgebraicExpression cofactor = calculateDeterminant(subMatrix);

        // 符号项：(-1)^(1+j+1) = (-1)^(j)
        AlgebraicExpression term = matrix[0][j] * cofactor;

        if (j % 2 == 0) {
            determinant = determinant + term;
        }
        else {
            determinant = determinant - term;
        }
    }

    return determinant;
}