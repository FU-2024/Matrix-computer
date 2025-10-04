#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// ��������
void printMatrix(const vector<vector<double>>& matrix);
double calculateDeterminant(vector<vector<double>> matrix);
vector<vector<double>> getSubMatrix(const vector<vector<double>>& matrix, int row, int col);

int main() {
    int n;

    cout << "=== ��������ʽ������ ===" << endl;
    cout << "���������Ľ���: ";
    cin >> n;

    if (n <= 0) {
        cout << "���󣺾����������Ϊ��������" << endl;
        return 1;
    }

    // ����n��n����
    vector<vector<double>> matrix(n, vector<double>(n));

    cout << "\n���������Ԫ�أ��������룩:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "�� " << i + 1 << " ��: ";
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }

    // ��ʾ����ľ���
    cout << "\n������ľ���Ϊ:" << endl;
    printMatrix(matrix);

    // ��������ʽ
    double determinant = calculateDeterminant(matrix);

    // ������
    cout << fixed << setprecision(6);
    cout << "\n����ʽֵ: " << determinant << endl;

    return 0;
}

// ��ӡ����
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

// ��ȡ�Ӿ���ȥ����row�к͵�col�У�
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

// �ݹ��������ʽ
double calculateDeterminant(vector<vector<double>> matrix) {
    int n = matrix.size();

    // 1��1��������
    if (n == 1) {
        return matrix[0][0];
    }

    // 2��2��������
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    // 3��3����������ʹ��Sarrus������ȻҲ�����õݹ飩
    if (n == 3) {
        return matrix[0][0] * matrix[1][1] * matrix[2][2] +
            matrix[0][1] * matrix[1][2] * matrix[2][0] +
            matrix[0][2] * matrix[1][0] * matrix[2][1] -
            matrix[0][2] * matrix[1][1] * matrix[2][0] -
            matrix[0][1] * matrix[1][0] * matrix[2][2] -
            matrix[0][0] * matrix[1][2] * matrix[2][1];
    }

    // n��n����n>3����ʹ��������˹չ���ص�һ��չ��
    double determinant = 0.0;
    for (int j = 0; j < n; j++) {
        // �����������ʽ
        vector<vector<double>> subMatrix = getSubMatrix(matrix, 0, j);
        double cofactor = calculateDeterminant(subMatrix);

        // �����(-1)^(1+j+1) = (-1)^(j)
        if (j % 2 == 0) {
            determinant += matrix[0][j] * cofactor;
        }
        else {
            determinant -= matrix[0][j] * cofactor;
        }
    }

    return determinant;
}