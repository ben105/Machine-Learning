#include <vector>

#ifndef MATRIX_H
#define MATRIX_H

template <class T>
class Matrix {
private:
	std::vector< std::vector<T> > _matrix;
	std::pair<int, int> _getDimensions() const;
	bool _sameDimensions(Matrix<T>& other) const;

protected:
	void _makeMatrix(std::pair<int, int> dimensions); 

public:
	Matrix();
	~Matrix();
	Matrix(int columns);
	Matrix(int rows, int columns);
	Matrix(std::pair<int, int> dimensions);
	Matrix(std::vector<T> array);
	Matrix(std::vector< std::vector<T> > arrays);

	Matrix<T> operator+ (Matrix<T>& rhs) const;
	Matrix<T> operator* (Matrix<T>& rhs) const;
	Matrix<T> operator- (Matrix<T>& rhs) const;

	void print() const;
};

#include "matrix.tpp"

#endif