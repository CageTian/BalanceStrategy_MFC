#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class Matrix
{
protected:
  double* memory_;
  double** matrix_;
  unsigned int n_;
  unsigned int m_;
protected:
  void create(unsigned int n, unsigned int m);
  void destroy();
  bool ludcmp(std::vector<int>& indx, int* d);
  bool lubksb(std::vector<int>& indx, std::vector<double>& b);
public:
  Matrix(unsigned int n = 0, unsigned int m = 0);
  Matrix(const Matrix& m);

  unsigned int getN() const { return n_; }
  unsigned int getM() const { return m_; }

  // Resize the matrix - destroys current contents
  void Resize(unsigned int n, unsigned int m) { destroy(); create(n, m); }

  void SetIdentity();

  void SetZero();

  void Transpose();

  // Invert an nxn matrix
  bool Invert();

  void Copy(const Matrix& M);

  // this = A * B
  virtual bool Mult(const Matrix& A, const Matrix& B);

  // this = M * this
  bool PreMult(const Matrix& M) { Matrix tmp(*this); return Mult(M, tmp); }

  // this = this * M
  bool PostMult(const Matrix& M) { Matrix tmp(*this); return Mult(tmp, M); }

  // Assignment operators
  Matrix& operator=(const Matrix& M) { Copy(M); return *this; }
  Matrix& operator*=(const Matrix& M) { PostMult(M); return *this; }

  // Binary operator
  Matrix operator*(const Matrix& M) { Matrix tmp; tmp.Mult(*this, M); return tmp; }

  double* operator[](int i) { return matrix_[i]; }
  const double* operator[](int i) const { return matrix_[i]; }

  virtual ~Matrix();
};

#endif
