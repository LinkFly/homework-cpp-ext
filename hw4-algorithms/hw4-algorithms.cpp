// hw4-algorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>
#include <numeric>

using namespace std;

template<typename T, template<typename T, typename = std::allocator<T>> class Container>
void insert_sorted(Container<T>& vec, const T& elt) {
	auto it = lower_bound(begin(vec), end(vec), elt);
	auto idx = distance(begin(vec), it);
	vec.resize(vec.size() + 1);
	auto itIns = begin(vec) + idx;
	copy(itIns, end(vec) - 1, itIns + 1);
	*itIns = elt;
};

void task1_test() {
	cout << "----------\nTask1: ";
	vector<int> vec = { 1, 2, 4, 5, 6 };
	insert_sorted(vec, 3);
	copy(begin(vec), end(vec), ostream_iterator<int>(cout, " "));
	cout << endl;
}

void task2_test() {
	cout << "-------------\nTask2: ";
	vector<double> vec(100);
	//mt19937 gen{random_device()()};
	//uniform_real_distribution<double> uid(-10, 10);
	const auto part = 2 * 3.14 / vec.size();
	double angle = 0;
	generate(begin(vec), end(vec), [&angle, part]() {
		auto res = 100 * sin(angle);
		angle += part;
		return res;
	});

	vector<int> vecInts(100);
	transform(begin(vec), end(vec), begin(vecInts), [](double val) {
		return static_cast<int>(val);
		});
	cout << "\n\nSignal vals:\n";
	copy(begin(vec), end(vec), ostream_iterator<double>(cout, " "));
	cout << "\n\nTruncated vals:\n";
	copy(begin(vecInts), end(vecInts), ostream_iterator<double>(cout, " "));

	double truncatedError = 0;
	for (size_t i = 0; i < vec.size(); ++i) {
		truncatedError += pow(vec[i] - vecInts[i], 2);
	}
	cout << "\n\nError after truncate: " << truncatedError << endl;
}

int main()
{
	task1_test();
	task2_test();
}
