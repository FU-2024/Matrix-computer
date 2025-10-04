#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// ��ʾ�������ʽ����
class AlgebraicExpression {
private:
    map<string, double> terms; // ��ϵ����keyΪ��������� "x", "xy", "1"(������)

public:
    AlgebraicExpression() {}

    // ���캯��������
    AlgebraicExpression(double constant) {
        if (constant != 0) {
            terms["1"] = constant;
        }
    }

    // ���캯������������
    AlgebraicExpression(const string& variable) {
        terms[variable] = 1.0;
    }

    // �ӷ����������
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

    // �������������
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

    // �˷����������
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

    // ���Գ���
    AlgebraicExpression operator*(double scalar) const {
        AlgebraicExpression result;
        for (const auto& term : terms) {
            result.terms[term.first] = term.second * scalar;
        }
        return result;
    }

    // ��Ԫ�������������Ա��ʽ
    friend AlgebraicExpression operator*(double scalar, const AlgebraicExpression& expr) {
        return expr * scalar;
    }

    // �ж��Ƿ�Ϊ0
    bool isZero() const {
        return terms.empty();
    }

    // ת��Ϊ�ַ���
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
                // ������
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
                // ������
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
    // �����������˱�����������ͺϲ���
    string multiplyVariables(const string& var1, const string& var2) const {
        if (var1 == "1") return var2;
        if (var2 == "1") return var1;

        // �ָ����������
        vector<string> vars;
        if (!var1.empty()) vars.push_back(var1);
        if (!var2.empty()) vars.push_back(var2);

        // ��ʵ�֣��������������ǵ��ַ�
        string result;
        for (const auto& v : vars) {
            result += v;
        }

        // ���ַ����򣨿�ѡ��ʹ������淶��
        sort(result.begin(), result.end());

        return result;
    }
};

// ��������
void printMatrix(const vector<vector<AlgebraicExpression>>& matrix);
AlgebraicExpression calculateDeterminant(vector<vector<AlgebraicExpression>> matrix);
vector<vector<AlgebraicExpression>> getSubMatrix(const vector<vector<AlgebraicExpression>>& matrix, int row, int col);
AlgebraicExpression parseExpression(const string& input);

int main() {
    int n;

    cout << "=== ���ž�������ʽ������ ===" << endl;
    cout << "֧���������ֺͱ���(x,y,z��)" << endl;
    cout << "���������Ľ���: ";
    cin >> n;

    if (n <= 0) {
        cout << "���󣺾����������Ϊ��������" << endl;
        return 1;
    }

    // ����n��n����
    vector<vector<AlgebraicExpression>> matrix(n, vector<AlgebraicExpression>(n));

    cout << "\n���������Ԫ�أ��������룬����x,y,z�ȱ�����:" << endl;
    cin.ignore(); // ������뻺����

    for (int i = 0; i < n; i++) {
        cout << "�� " << i + 1 << " ��: ";
        string line;
        getline(cin, line);

        stringstream ss(line);
        for (int j = 0; j < n; j++) {
            string element;
            ss >> element;
            matrix[i][j] = parseExpression(element);
        }
    }

    // ��ʾ����ľ���
    cout << "\n������ľ���Ϊ:" << endl;
    printMatrix(matrix);

    // ��������ʽ
    cout << "\n��������ʽ..." << endl;
    AlgebraicExpression determinant = calculateDeterminant(matrix);

    // ������
    cout << "\n����ʽ���ʽ: " << determinant.toString() << endl;

    return 0;
}

// ���������ַ���Ϊ�������ʽ
AlgebraicExpression parseExpression(const string& input) {
    // ����Ƿ�Ϊ������
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
        // ���������ǵ�������
        return AlgebraicExpression(input);
    }
}

// ��ӡ����
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

// ��ȡ�Ӿ���ȥ����row�к͵�col�У�
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

// �ݹ��������ʽ
AlgebraicExpression calculateDeterminant(vector<vector<AlgebraicExpression>> matrix) {
    int n = matrix.size();

    // 1��1��������
    if (n == 1) {
        return matrix[0][0];
    }

    // 2��2��������
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    // n��n����n>=3����ʹ��������˹չ���ص�һ��չ��
    AlgebraicExpression determinant;

    for (int j = 0; j < n; j++) {
        // �����������ʽ
        vector<vector<AlgebraicExpression>> subMatrix = getSubMatrix(matrix, 0, j);
        AlgebraicExpression cofactor = calculateDeterminant(subMatrix);

        // �����(-1)^(1+j+1) = (-1)^(j)
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