#pragma once

#include <vector>

template<typename T>
T& min_el(const std::vector<T>& vec);

template float& min_el(const std::vector<float>& vec);

template<typename T>
T& max_el(const std::vector<T>& vec);

template float& max_el(const std::vector<float>& vec);

extern int min_el2(const std::vector<int>& vec);
