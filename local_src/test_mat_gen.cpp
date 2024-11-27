#include <iostream>
#include <random>
#include <vector>

using namespace std;

template <typename T> vector<vector<T>> GetMat(int rows, int cols) {
  default_random_engine generator(random_device{}());
  binomial_distribution<int> distribution(
      255, 0.5); // Distribution for random numbers between 0 and 255
  vector<vector<T>> mat(rows, vector<T>(cols));

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int tmp = distribution(generator) - 128; // Convert to range -128 to 127
      mat[i][j] = tmp;
    }
  }

  return mat;
}

template <typename T>
void MatMul(const vector<vector<T>> &A, const vector<vector<T>> &B,
            vector<vector<T>> &C) {
  int rows = A.size();
  int inner = A[0].size();
  int cols = B[0].size();

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      C[i][j] = 0;
      for (int k = 0; k < inner; ++k) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

template <typename T>
void GenerateOMat(vector<vector<T>> &O_mat, const vector<vector<T>> &W_mat,
                  const vector<vector<T>> &I_mat,
                  const vector<vector<T>> &B_mat, bool IsRelu, int BiasShift,
                  int AccumShift, int AccumMul) {
  MatMul(W_mat, I_mat, O_mat);

  int rows = O_mat.size();
  int cols = O_mat[0].size();

  for (int j = 0; j < cols; ++j) {
    for (int i = 0; i < rows; ++i) {
      int tmp = B_mat[i][0];
      O_mat[i][j] = O_mat[i][j] + (tmp << BiasShift);
      if (IsRelu && O_mat[i][j] < 0)
        O_mat[i][j] = 0;
      else {
        O_mat[i][j] = O_mat[i][j] * AccumMul;
        O_mat[i][j] = O_mat[i][j] >> AccumShift;
        if (O_mat[i][j] > 127)
          O_mat[i][j] = 127;
        if (O_mat[i][j] < -128)
          O_mat[i][j] = -128;
      }
    }
  }
}

template <typename T>
void PrintMatrix(const string &name, const vector<vector<T>> &mat) {
  int rows = mat.size();
  int cols = mat[0].size();
  cout << "int " << name << "[" << rows << "][" << cols << "];\n";
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      cout << name << "[" << i << "][" << j << "] = " << mat[i][j] << "; ";
    }
  }
  cout << "\n";
}

int main() {
  const int N = 32;
  const int M = 32;

  // Generate matrices
  auto W_mat = GetMat<int>(N, N);
  auto I_mat = GetMat<int>(N, M);
  auto B_mat = GetMat<int>(N, 1);

  vector<vector<int>> O_mat(N, vector<int>(M));
  GenerateOMat(O_mat, W_mat, I_mat, B_mat, true, 6, 10, 93);

  // Print matrices
  PrintMatrix("W_mat", W_mat);
  PrintMatrix("I_mat", I_mat);
  PrintMatrix("B_mat", B_mat);
  PrintMatrix("O_mat", O_mat);

  return 0;
}
