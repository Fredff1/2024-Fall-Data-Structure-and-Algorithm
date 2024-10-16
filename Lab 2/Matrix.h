#ifndef MATRIX_H
#define MATRIX_H

#include "Sort.h"
#include "Utilility.h"

#include <type_traits>


/* Make sure T can be operated with these operators*/
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
class Matrix {
  private:
    vector<vector<T>> data;
    int row_size;
    int col_size;

    /*Get the size to extend it to square*/
    const int next_power_of_two(int x) const {
        int power = 1;
        while (power < x) {
            power *= 2;
        }
        return power;
    }

    /* The recursing helper function for Strassen mutipliaction*/
    Matrix<T> Strassen_mutipliaction_recurse_helper(const Matrix<T> &target_matrix_1, const Matrix<T> &target_matrix_2, int stop_size = -1) const {
        bool use_mix = true;
        if (stop_size < 1) {
            stop_size = 1;
            use_mix = false;
        }

        if (target_matrix_1.get_col() != target_matrix_2.get_row()) {
            throw std::invalid_argument("Submatrix dimensions do not match for multiplication.");
        }

        int size = target_matrix_1.get_row();

        if (size <= stop_size) {
            return target_matrix_1 * target_matrix_2;

        } else {

            /*To get the 8 submatrixes*/
            Matrix<T> A11 = target_matrix_1.get_submatrix(0, 0, size / 2);
            Matrix<T> A12 = target_matrix_1.get_submatrix(0, size / 2, size / 2);
            Matrix<T> A21 = target_matrix_1.get_submatrix(size / 2, 0, size / 2);
            Matrix<T> A22 = target_matrix_1.get_submatrix(size / 2, size / 2, size / 2);

            Matrix<T> B11 = target_matrix_2.get_submatrix(0, 0, size / 2);
            Matrix<T> B12 = target_matrix_2.get_submatrix(0, size / 2, size / 2);
            Matrix<T> B21 = target_matrix_2.get_submatrix(size / 2, 0, size / 2);
            Matrix<T> B22 = target_matrix_2.get_submatrix(size / 2, size / 2, size / 2);

            /*The 7 matrix to compute the result according to the textbook
              Of course,I have changed some of the sequences, so it is not exactly the same
            */
            Matrix<T> P1 = Strassen_mutipliaction_recurse_helper((A11 + A22) ,(B11 + B22), stop_size);
            Matrix<T> P2 = Strassen_mutipliaction_recurse_helper((A21 + A22), B11, stop_size);
            Matrix<T> P3 = Strassen_mutipliaction_recurse_helper(A11, (B12 - B22), stop_size);
            Matrix<T> P4 = Strassen_mutipliaction_recurse_helper(A22, (B21 - B11), stop_size);
            Matrix<T> P5 = Strassen_mutipliaction_recurse_helper((A11 + A12), B22, stop_size);
            Matrix<T> P6 = Strassen_mutipliaction_recurse_helper((A21 - A11), (B11 + B12), stop_size);
            Matrix<T> P7 = Strassen_mutipliaction_recurse_helper((A12 - A22), (B21 + B22), stop_size);

            /*The 4 submatrixes of the result*/
            Matrix<T> C11 = P1 + P4 - P5 + P7;
            Matrix<T> C12 = (P3 + P5);
            Matrix<T> C21 = (P2 + P4);
            Matrix<T> C22 = P1 + P3 - P2 + P6;

            /*Merge the results*/
            Matrix<T> result(size, size);
            result.set_submatrix(0, 0, C11);
            result.set_submatrix(0, size / 2, C12);
            result.set_submatrix(size / 2, 0, C21);
            result.set_submatrix(size / 2, size / 2, C22);
            return result;
        }
    }

  public:
    Matrix(int rows, int cols) : row_size(rows), col_size(cols) {
        data.resize(rows, vector<T>(cols, 0));
    }

    Matrix(int rows, int cols, bool flag) : row_size(rows), col_size(cols) {
        if (flag == true) {
            data.resize(rows, vector<T>(cols, 0));
        }
    }

    
    

    vector<vector<T>> &getdata() {
        return this->data;
    }

    void setAllZero() {
        for (auto &rows : data) {
            for (auto &num : data[rows]) {
                num = 0;
            }
        }
    }

    const int get_row() const {
        return this->row_size;
    }

    const int get_col() const {
        return this->col_size;
    }

    /*
    ###
    Overloading some operators 
    
    ###
    */

    /*Both you can get the niumber*/
    T &operator()(int row, int col) {
        return data[row][col];
    }

    std::vector<T> &operator[](int row) {
        return data[row];
    }

    const std::vector<T> &operator[](int row) const {
        return data[row];
    }

    
    bool operator==(const Matrix<T> &other) const {
        if (row_size != other.get_row() || col_size != other.get_col()) {
            return false;
        }

        for (int i = 0; i < row_size; ++i) {
            for (int j = 0; j < col_size; ++j) {
                if (data[i][j] != other[i][j]) {
                    std::printf("Difference detected at position (%d,%d)",i,j);
                    return false;
                }
            }
        }
        return true;
    }

    Matrix<T> operator+(const Matrix<T> &other) const {
        if (row_size != other.get_row() || col_size != other.get_col()) {
            throw std::invalid_argument("Matrix dimensions do not match for addition operator.");
        }

        Matrix<T> result(row_size, col_size);
        for (int i = 0; i < row_size; ++i) {
            for (int j = 0; j < col_size; ++j) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix<T> operator-(const Matrix<T> &other) const {
        if (row_size != other.get_row() || col_size != other.get_col()) {
            throw std::invalid_argument("Matrix dimensions do not match for subtraction operator.");
        }

        Matrix<T> result(row_size, col_size);
        for (int i = 0; i < row_size; ++i) {
            for (int j = 0; j < col_size; ++j) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    Matrix<T> &operator+=(const Matrix<T> &other) {
        if (row_size != other.get_row() || col_size != other.get_col()) {
            throw std::invalid_argument("Matrix dimensions do not match for addition operator.");
        }

        for (int i = 0; i < row_size; ++i) {
            for (int j = 0; j < col_size; ++j) {
                data[i][j] += other[i][j];
            }
        }
        return *this;
    }

    Matrix<T> &operator-=(const Matrix<T> &other) {
        if (row_size != other.get_row() || col_size != other.get_col()) {
            throw std::invalid_argument("Matrix dimensions do not match for subtraction operator.");
        }

        for (int i = 0; i < row_size; ++i) {
            for (int j = 0; j < col_size; ++j) {
                data[i][j] -= other[i][j];
            }
        }
        return *this;
    }

    /*Normal multiplication */
    Matrix<T> operator*(const Matrix<T> &other) const {
        if (col_size != other.get_row()) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication operator.");
        }
        Matrix<T> result(row_size, other.get_col());
        for (int i = 0; i < row_size; i++) {
            for (int j = 0; j < other.get_col(); j++) {
                for (int k = 0; k < col_size; k++) {
                    result[i][j] += data[i][k] * other[k][j];
                }
            }
        }
        return result;
    }

    /*Normal multiplication for a function*/
    Matrix<T> mutiplcation(const Matrix<T> &other) const {
        if (col_size != other.get_row()) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication operator.");
        }
        Matrix<T> result(row_size, other.get_col());
        for (int i = 0; i < row_size; i++) {
            for (int j = 0; j < other.get_col(); j++) {
                for (int k = 0; k < col_size; k++) {
                    result[i][j] += data[i][k] * other[k][j];
                }
            }
        }
        return result;
    }

    /*print the matrix*/
    void print() const {
        cout << "-----------------------------------------------------" << endl;
        for (auto &rows : data) {
            for (auto &num : rows) {
                cout << num << ' ';
            }
            cout << endl;
        }
        cout << "-----------------------------------------------------" << endl;
    }

    /*Get a random matrix*/
    static Matrix<T> getRandomMatrix(int rows, int cols, T start_num, T end_num) {
        Matrix<T> matrix_1(rows, cols);
        for (int i = 0; i < matrix_1.get_row(); i++) {
            for (int j = 0; j < matrix_1.get_col(); j++) {
                matrix_1[i][j] = RandomGenerator::getRandomValue<T>(start_num, end_num);
            }
        }
        return matrix_1;
    }

    bool is_square() const {
        return row_size == col_size;
    }


    /*These are helper functions for Strassen algorithm*/

    Matrix<T> get_submatrix(int start_row, int start_col, int size) const {
        Matrix<T> submatrix(size, size); 
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                submatrix[i][j] = data[start_row + i][start_col + j]; 
            }
        }
        return submatrix; 
    }

    
    /*To merge the submatrix to the original one*/
    void set_submatrix(int start_row, int start_col, Matrix<T> &sub) {

        for (int i = start_row; i < start_row + sub.get_row(); i++) {
            for (int j = start_col; j < start_col + sub.get_col(); j++) { /*Get the size to extend it to square*/
                data[i][j] = sub[i - start_row][j - start_col];
            }
        }
    }

    /*The new matrix has 0 for padded ones*/
    Matrix<T> pad_to_square(int min) const {
        int max_dim = std::max(row_size, col_size);
        if (max_dim < min) {
            max_dim = min;
        }

        int new_size = next_power_of_two(max_dim);

        if (new_size == row_size && new_size == col_size) {
            return *this;
        }

        Matrix<T> padded_matrix(new_size, new_size);

        for (int i = 0; i < row_size; ++i) {
            for (int j = 0; j < col_size; ++j) {
                padded_matrix[i][j] = data[i][j];
            }
        }

        return padded_matrix;
    }

    /*To restore the original matrix*/
    Matrix<T> remove_padding(int row_size, int col_size) {
        Matrix<T> resized_matrix(row_size, col_size);
        for (int i = 0; i < row_size; ++i) {
            for (int j = 0; j < col_size; ++j) {
                resized_matrix[i][j] = data[i][j];
            }
        }
        return resized_matrix;
    }

    /*The formal function for Strassen algorithm*/
    Matrix<T> Strassen_mutipliaction(const Matrix &other, int change_size) const {
        /*Validate input*/
        if (col_size != other.get_row()) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication operator.");
        }
        int max_dim_this = std::max(row_size, col_size);
        int max_dim_other = std::max(other.get_row(), other.get_col());
        int max_dim = std::max(max_dim_this, max_dim_other);

        /*Standarize input*/
        Matrix<T> padded_matrix_1 = this->pad_to_square(max_dim);
        Matrix<T> padded_matrix_2 = other.pad_to_square(max_dim);

        /*Run the function*/
        Matrix result = Strassen_mutipliaction_recurse_helper(padded_matrix_1, padded_matrix_2, change_size);

        /*Restore the result*/
        return result.remove_padding(row_size, other.get_col());
    }

    
};

#endif
