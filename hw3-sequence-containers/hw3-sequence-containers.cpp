// hw3-sequence-containers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <forward_list>
#include <vector>

using namespace std;

void add_average(forward_list<float>& list) {
	float sum = .0f;
	int i = 0;
	forward_list<float>::const_iterator last;
	for (auto it = cbegin(list); it != cend(list); ++it) {
		sum += *it;
		++i;
		last = it;
	}
	if (i != 0) {
		list.insert_after(last, sum / i);
	}
}

struct Matrix {
	int nrowcol;
	int curRowCol;
	int** data;
	Matrix(int nrowcol): nrowcol{nrowcol}, curRowCol{nrowcol} {
		data = new int*[nrowcol];
		for(int i = 0; i < nrowcol; ++i)
			data[i] = new int[nrowcol];
	}
	Matrix(vector<vector<int>> data) : Matrix(data.size()) {
		for(size_t i = 0; i < data.size(); ++i)
			for(size_t j = 0; j < data[i].size(); ++j)
				set(i, j, data[i][j]);
			
	}
	~Matrix() {
		for(int i = 0; i < nrowcol; ++i)
			delete[] data[i];
		delete[] data;
	}
	int get(int i, int j) const {
		return data[i][j];
	}
	void set(int i, int j, int num) {
		data[i][j] = num;
	}

	// Get matrix without i-row and j-col
	static void GetMatr(const Matrix& mtxSrc, Matrix& mtx, int i, int j, int m) {
		int ki, kj, di, dj;
		di = 0;
		for (ki = 0; ki < m - 1; ki++) {
			if (ki == i) di = 1;
			dj = 0;
			for (kj = 0; kj < mtxSrc.curRowCol - 1; kj++) {
				if (kj == j) dj = 1;
				mtx.set(ki, kj, mtxSrc.get(ki + di, kj + dj));
			}
		}
	}

	static int staticGetDet(Matrix& mtx) {
		int i, j, d, k;
		j = 0; d = 0;
		k = 1;
		Matrix mtxCut(mtx.curRowCol - 1);
		if (mtx.curRowCol < 1) {
			cerr << "Determinant evaluating unreal";
			exit(-1);
		}
		if (mtx.curRowCol == 1) {
			d = mtx.get(0, 0);
			return(d);
		}
		if (mtx.curRowCol == 2) {
			d = mtx.get(0, 0) * mtx.get(1, 1) - (mtx.get(1, 0) * mtx.get(0, 1));
			return(d);
		}
		if (mtx.curRowCol > 2) {
			for (i = 0; i < mtx.curRowCol; i++) {
				GetMatr(mtx, mtxCut, i, 0, mtx.curRowCol);
				d = d + k * mtx.get(i, 0) * staticGetDet(mtxCut);
				k = -k;
			}
		}
		return d;
	}

	int getDet() {
		return staticGetDet(*this);
	}
};

class OwnContainer;

template<typename ValueType>
class OwnIterator : public std::iterator<std::input_iterator_tag, ValueType>
{
	friend class OwnContainer;
	int cur;
public:
	OwnIterator(ValueType cur);

	bool operator!=(OwnIterator const& other) const;
	bool operator==(OwnIterator const& other) const;
	ValueType operator*() const;
	OwnIterator& operator++();
private:
	ValueType* p;
};

template<typename ValueType>
OwnIterator<ValueType>::OwnIterator(ValueType cur) : cur{ cur } {}

template<typename ValueType>
bool OwnIterator<ValueType>::operator!=(OwnIterator const& other) const
{
	return cur != other.cur;
}

template<typename ValueType>
bool OwnIterator<ValueType>::operator==(OwnIterator const& other) const
{
	return cur == other.cur;
}

template<typename ValueType>
ValueType OwnIterator<ValueType>::operator*() const
{
	return cur;
}

template<typename ValueType>
OwnIterator<ValueType>& OwnIterator<ValueType>::operator++()
{
	++cur;
	return *this;
}

class OwnContainer
{
	int min, max;
public:
	using iterator = OwnIterator<int>;
	using const_iterator = OwnIterator<const int>;

	OwnContainer(int min, int max): min{min}, max{max} {}

	iterator begin() {
		return {min};
	}
	iterator end() {
		return {max + 1};
	}
};

int main()
{
    forward_list<float> list{1, 2, 3, 4};
	add_average(list);
	for(auto& x : list) 
		cout << x << " ";
	cout << endl;

	Matrix matrix(
		{
			{2, 4, 3},
			{5, 7, 8},
			{6, 9, 1}
		}
	);
	int det = matrix.getDet();
	cout << "Det({2, 4, 3}, {5, 7, 8}, {6, 9, 1}) = " << det << endl;
	OwnContainer container(-5, 5);
	for (int it : container) {
		cout << it << " ";
	}
}
