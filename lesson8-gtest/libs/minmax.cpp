#include <vector>
#include <algorithm>
#include "minmax.h"

template<typename T>
T& min_el(const std::vector<T>& vec) {
	return const_cast<T&>(*min_element(begin(vec), end(vec)));
}
//extern template float& min_el(const std::vector<float>& vec);

template<typename T>
T& max_el(const std::vector<T>& vec) {
	return const_cast<T&>(*max_element(begin(vec), end(vec)));
}

extern int min_el2(const std::vector<int>& vec)
{
	return const_cast<int&>(*max_element(begin(vec), end(vec)));
}
