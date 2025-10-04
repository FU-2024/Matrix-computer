#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// 函数声明
void printMatrix(const vector<vector<double>>& matrix);
double calculateDeterminant(vector<vector<double>> matrix);
vector<vector<double>> getSubMatrix(const vector<vector<double>>& matrix, int row, int col);

int main() {
    int n;

    cout << "=== 矩阵行列式计算器 ===" << endl;
    cout << "请输入矩阵的阶数: ";
    cin >> n;

    if (n <= 0) {
        cout << "错误：矩阵阶数必须为正整数！" << endl;
        return 1;
    }

    // 创建n×n矩阵
    vector<vector<double>> matrix(n, vector<double>(n));

    cout << "\n请输入矩阵元素（按行输入）:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "第 " << i + 1 << " 行: ";
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }

    // 显示输入的矩阵
    cout << "\n您输入的矩阵为:" << endl;
    printMatrix(matrix);

    // 计算行列式
    double determinant = calculateDeterminant(matrix);

    // 输出结果
    cout << fixed << setprecision(6);
    cout << "\n行列式值: " << determinant << endl;

    return 0;
}

// 打印矩阵
void printMatrix(const vector<vector<double>>& matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        cout << "| ";
        for (int j = 0; j < n; j++) {
            cout << setw(8) << matrix[i][j] << " ";
        }
        cout << "|" << endl;
    }
}

// 获取子矩阵（去掉第row行和第col列）
vector<vector<double>> getSubMatrix(const vector<vector<double>>& matrix, int row, int col) {
    int n = matrix.size();
    vector<vector<double>> subMatrix(n - 1, vector<double>(n - 1));

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
double calculateDeterminant(vector<vector<double>> matrix) {
    int n = matrix.size();

    // 1×1矩阵的情况
    if (n == 1) {
        return matrix[0][0];
    }

    // 2×2矩阵的情况
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    // 3×3矩阵的情况（使用Sarrus法则，虽然也可以用递归）
    if (n == 3) {
        return matrix[0][0] * matrix[1][1] * matrix[2][2] +
            matrix[0][1] * matrix[1][2] * matrix[2][0] +
            matrix[0][2] * matrix[1][0] * matrix[2][1] -
            matrix[0][2] * matrix[1][1] * matrix[2][0] -
            matrix[0][1] * matrix[1][0] * matrix[2][2] -
            matrix[0][0] * matrix[1][2] * matrix[2][1];
    }

    // n×n矩阵（n>3）：使用拉普拉斯展开沿第一行展开
    double determinant = 0.0;
    for (int j = 0; j < n; j++) {
        // 计算代数余子式
        vector<vector<double>> subMatrix = getSubMatrix(matrix, 0, j);
        double cofactor = calculateDeterminant(subMatrix);

        // 符号项：(-1)^(1+j+1) = (-1)^(j)
        if (j % 2 == 0) {
            determinant += matrix[0][j] * cofactor;
        }
        else {
            determinant -= matrix[0][j] * cofactor;
        }
    }

    return determinant;
}