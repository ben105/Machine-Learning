#include "matrix.h"
#include <vector>
#include <assert.h>
#include <iostream>

using namespace std;

class UnmatchedMatrixException: public exception {
	virtual const char* what() const noexcept {
		return "Matrix operation on unmatched dimensions";
	}
} UNMATCHED_DIMENSIONS;

class VariableMatrixException: public exception {
	virtual const char* what() const noexcept {
		return "Matrix cannot contain variable subvector lengths.";
	}
} VARIABLE_VECTOR_LENGTHS;


template <class T>
void Matrix<T>::_makeMatrix(std::pair<int, int> dimensions) {
	assert(dimensions.first > 0);
	assert(dimensions.second > 0);

	this->_matrix = vector< vector<T> >();
	for (unsigned i = 0; i < dimensions.first; ++i){
		// Create a row with 0's the length of pair.second.
		vector<T> row(dimensions.second);
		this->_matrix.push_back(row);
	}
}

template <class T>
Matrix<T>::Matrix() {
	this->_makeMatrix(make_pair(1, 1));
}

template <class T>
Matrix<T>::~Matrix() {
}

template <class T>
Matrix<T>::Matrix(int columns) {
	this->_makeMatrix(make_pair(1, columns));
}

template <class T>
Matrix<T>::Matrix(int rows, int columns) {
	this->_makeMatrix(make_pair(rows, columns));
}

template <class T>
Matrix<T>::Matrix(pair<int, int> dimensions) {
	this->_makeMatrix(dimensions);
}

template <class T>
Matrix<T>::Matrix(vector<T> array) {
	assert(array.size() > 0);
	this->_matrix = vector< vector<T> >();
	this->_matrix.push_back(array);
}

template <class T>
Matrix<T>::Matrix(vector< vector<T> > arrays) {
	assert(arrays.size() > 0);

	// We want to throw an exception if one of the vectors in the 
	// array is a different length.
	// Remember the size of the arrays. Start with the first.
	unsigned mem_size = arrays[0].size();
	for (typename vector< vector<T> >::iterator it = arrays.begin(); it != arrays.end(); ++it) {
		if (mem_size != it->size()){
			throw VARIABLE_VECTOR_LENGTHS;
		}
	}

	this->_matrix = arrays;
}




template <class T>
bool Matrix<T>::_sameDimensions(Matrix<T>& other) const {
	pair<int, int> thisDimensions = this->_getDimensions();
	pair<int, int> otherDimensions = other._getDimensions();
	return (thisDimensions.first == otherDimensions.first && 
		thisDimensions.second == otherDimensions.second);
}

template <class T>
Matrix<T> Matrix<T>::operator+ (Matrix<T>& rhs) const{
	if (!this->_sameDimensions(rhs)) {
		throw;
	}
	Matrix<T> newMatrix = Matrix<T>(this->_getDimensions());
	for (unsigned i = 0; i < this->_matrix.size(); ++i) {
		for (unsigned j = 0; j < this->_matrix[i].size(); ++j) {
			newMatrix._matrix[i][j] = this->_matrix[i][j] + rhs._matrix[i][j];
		}
	}
	return newMatrix;
}

template <class T>
Matrix<T> Matrix<T>::operator* (Matrix<T>& rhs) const{
	if (!this->_sameDimensions(rhs)) {
		throw;
	}
	Matrix<T> newMatrix = Matrix<T>(this->_getDimensions());
	for (unsigned i = 0; i < this->_matrix.size(); ++i) {
		for (unsigned j = 0; j < this->_matrix[i].size(); ++j) {
			newMatrix._matrix[i][j] = this->_matrix[i][j] * rhs._matrix[i][j];
		}
	}
	return newMatrix;
}

template <class T>
Matrix<T> Matrix<T>::operator- (Matrix<T>& rhs) const{
	if (!this->_sameDimensions(rhs)) {
		throw;
	}
	Matrix<T> newMatrix = Matrix<T>(this->_getDimensions());
	for (unsigned i = 0; i < this->_matrix.size(); ++i) {
		for (unsigned j = 0; j < this->_matrix[i].size(); ++j) {
			newMatrix._matrix[i][j] = this->_matrix[i][j] - rhs._matrix[i][j];
		}
	}
	return newMatrix;
}

template <class T>
pair<int, int> Matrix<T>::_getDimensions() const{
	int rows = this->_matrix.size();
	int columns = this->_matrix[0].size();
	return make_pair(rows, columns);
}


template <class T>
void Matrix<T>::print() const {
	for (unsigned i = 0; i < this->_matrix.size(); ++i) {
		for (unsigned j = 0; j < this->_matrix[i].size(); ++j) {
			cout << this->_matrix[i][j] << " ";
		}
		cout << endl;
	}
}
