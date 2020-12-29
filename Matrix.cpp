#include "Matrix.h"

using namespace std;

void Matrix::rawClean()
{
    for (int i = 0; i < this->size; i++)
        delete[] this->M[i];
    delete[] this->M;
}

void Matrix::rawCopy(const Matrix& that)
{
    this->size = that.size;
    this->M = new double* [this->size];
    for (int i = 0; i < this->size; i++)
        this->M[i] = new double[this->size];
    for (int i = 0; i < this->size; i++)
        for (int j = 0; j < this->size; j++)
            this->M[i][j] = that.M[i][j];
}


Matrix::Matrix(const Matrix& that) { rawCopy(that); }
Matrix::Matrix() { size = 0; this->M = nullptr; }
Matrix::~Matrix() { rawClean(); }

Matrix& Matrix::operator = (const Matrix& that) {
    if (this != &that) {
        rawClean();
        rawCopy(that);
    }
    return *this;
}

size_t Matrix::getSize() const { return size; }
double*& Matrix::operator [] (const int& r) { return M[r]; } // с изменением
double*& Matrix::get(const int& r) const { return M[r]; }


Matrix::Matrix(const size_t& s) : size(s) {
    M = new double*[size];
    for (int i = 0; i < size; i++)
        M[i] = new double[size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (i == j)
                M[i][j] = 1;
            else
                M[i][j] = 0;
}

Matrix::Matrix(const initializer_list<Vector>& list) : size(list.size())
{
    M = new double* [size];
    for (int i = 0; i < size; i++)
        M[i] = new double[size];
    size_t i = 0;
    for (const auto& v : list)
    {
        for (size_t j = 0; j < size; j++)
            M[i][j] = v.get(j);
        i++;
    }
}

Matrix& Matrix::operator = (const initializer_list<Vector>& list)
{
    *this = Matrix(list);
    return *this;
}

Matrix Matrix::H() { // нахождение матрицы отражений
    Matrix result(size);
    Matrix R = *this;
    for (int i = 0; i < size - 1; i++) {
        double* t = new double[size - i];
        for (int j = 0; j < size - i; j++)
            t[j] = R.get(j + i)[i];
        Vector b(this->size - i, t); // вектор b - первый столбец нашей матрицы размера n - i, где i - итерация главного цикла for
        for (int j = 0; j < size - i; j++)
            if (j == 0)
                t[j] = 1;
            else
                t[j] = 0;
        Vector c(size - i, t); // вектор c - единично-координатный вектор
        delete[] t;
        Vector temp = b - (b.abs() * c);
        Vector w = (temp / sqrt(2 * (b * temp))); // ключевой вектор, из которого строится матрица отражений для каждой итерации
        Matrix Omega(size - i);
        for (int j = 0; j < size - i; j++)
            for (int k = 0; k < size - i; k++)
                Omega[j][k] = w.get(j) * w.get(k);
        Matrix E(size - i);
        Matrix EU = E - (2 * Omega); // матрица отражений для каждой итерации
        Matrix U(size); // обобщённая матрица отражений (размера n)
        for (int j = 0; j < size; j++)
            for (int k = 0; k < size; k++)
                if (j >= i && k >= i) {
                    U[j][k] = EU.get(j - i)[k - i];
                } else {
                    if (j == k)
                        U[j][k] = 1;
                    else
                        U[j][k] = 0;
                }
        result = U * result; // формирование матрицы отражений в каждой итерации цикла
        R = U * R;
    }
    return result;
}

// стандартные операторы для работы с матрицами
Matrix Matrix::operator + (const Matrix& that) {
    Matrix result(size);
    for (int i = 0; i < result.size; i++)
        for (int j = 0; j < result.size; j++)
            result.M[i][j] = M[i][j] + that.M[i][j];
    return result;
}

Matrix Matrix::operator - (const Matrix& that) {
    Matrix result(size);
    for (int i = 0; i < result.size; i++)
        for (int j = 0; j < result.size; j++)
            result.M[i][j] = M[i][j] - that.M[i][j];
    return result;
}

Matrix Matrix::operator * (const Matrix& that) {
    Matrix result(size);
    for (int i = 0; i < result.size; i++) {
        result.M[i][i] = 0;
        for (int j = 0; j < result.size; j++)
            for (int k = 0; k < result.size; k++)
                result.M[i][j] += M[i][k] * that.M[k][j];
    }
    return result;
}

Matrix Matrix::operator * (const double& coeff)
{
    Matrix result(size);
    for (int i = 0; i < result.size; i++)
        for (int j = 0; j < result.size; j++)
            result.M[i][j] = M[i][j] * coeff;
    return result;
}

Matrix operator * (const double& coeff, const Matrix& that) {
    Matrix result(that.getSize());
    for (int i = 0; i < result.getSize(); i++)
        for (int j = 0; j < result.getSize(); j++)
            result[i][j] = that.get(i)[j] * coeff;
    return result;
}

Matrix Matrix::operator !()
{
    Matrix result(size);
    for (int i = 0; i < result.size; i++)
        for (int j = 0; j < result.size; j++)
            result.M[i][j] = M[j][i];
    return result;
}

bool Matrix::operator == (const Matrix& that) const
{
    bool result = true;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (M[i][j] != that.M[i][j])
            {
                result = false;
                break;
            }
    return result;
}

bool Matrix::operator != (const Matrix& that) const
{
    bool result = false;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (M[i][j] != that.M[i][j])
            {
                result = true;
                break;
            }
    return result;
}

void Matrix::swap(const int& i1, const int& i2) { // смена строк местами
    for (int i = 0; i < size; i++) {
        double temp = M[i1][i];
        M[i1][i] = M[i2][i];
        M[i2][i] = temp;
    }
}

// вычисление определителя матрицы
double Matrix::det() const{
    Matrix T = *this;
    double result = 1;
    for (int i = 0; i < T.size - 1; i++) {
        if (!T.M[i][i])
            for (int j = i + 1; j < T.size; j++) {
                if (T.M[j][i]) {
                    T.swap(j, i);
                    break;
                }
            }
        for (int j = i + 1; j < T.size; j++) {
            double temp = T.M[j][i] / T.M[i][i];
            for (int k = 0; k < T.size; k++)
                T.M[j][k] -= T.M[i][k] * temp;
        }
    }
    for (int i = 0; i < T.size; i++)
        result *= T.M[i][i];
    return result;
}

// нахождение обратной матрицы методом Жордана-Гаусса
Matrix Matrix::reflect()
{
    Matrix E(this->size);
    Matrix T = *this;
    // прямой ход метода Гаусса
    for (int i = 0; i < T.size - 1; i++) {
        if (!T.M[i][i])
            for (int j = i + 1; j < T.size; j++) {
                if (T.M[j][i]) {
                    T.swap(j, i);
                    E.swap(j, i);
                    break;
                }
            }
        for (int j = i + 1; j < T.size; j++) {
            double temp = T.M[j][i] / T.M[i][i];
            for (int k = 0; k < T.size; k++) {
                E.M[j][k] -= E.M[i][k] * temp;
                T.M[j][k] -= T.M[i][k] * temp;
            }
        }
    }
    // обратный ход метода Гаусса
    for (int i = T.size - 1; i > 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            double temp = T.M[j][i] / T.M[i][i];
            for (int k = T.size - 1; k >= 0; k--) {
                E.M[j][k] -= E.M[i][k] * temp;
                T.M[j][k] -= T.M[i][k] * temp;
            }
        }
    }
    for (int i = 0; i < T.size; i++) {
        for (int j = 0; j < this->size; j++)
            E.M[i][j] /= T.M[i][i];
        T.M[i][i] = 1;
    }
    return E;
}

Matrix Matrix::diag()
{
    Matrix result(size);
    for (int i = 0; i < size; i++)
        result[i][i] = M[i][i];
    return result;
}

Matrix Matrix::lowerTriangle()
{
    Matrix result(size);
    for (int i = 0; i < size; i++)
        for (int j = i; j < size; j++)
            if (i == j)
                result[j][i] = 0;
            else
                result[j][i] = M[j][i];
    return result;
}

Matrix Matrix::upperTriangle()
{
    Matrix result(size);
    for (int i = 0; i < size; i++)
        for (int j = i; j < size; j++)
            if (i == j)
                result[i][j] = 0;
            else
                result[i][j] = M[i][j];
    return result;
}

// операторы ввода/вывода матрицы
istream& operator >> (istream& in, Matrix& that)
{
    for (int i = 0; i < that.getSize(); i++)
        for (int j = 0; j < that.getSize(); j++)
            in >> that[i][j];
    return in;
}

ostream& operator << (ostream& out, const Matrix& that)
{
    for (int i = 0; i < that.getSize(); i++)
    {
        for (int j = 0; j < that.getSize(); j++)
            out << that.get(i)[j] << '\t';
        out << endl;
    }
    out << endl;
    return out;
}

Vector operator * (const Matrix& that, const Vector& v)
{
    Vector result(v.getSize());
    for (int i = 0; i < result.getSize(); i++)
    {
        result[i] = 0;
        for (int j = 0; j < result.getSize(); j++)
            result[i] += that.get(i)[j] * v.get(j);
    }
    return result;
}

// QR метод нахождения собственных чисел и векторов матрицы
void Matrix::QR()
{

}
