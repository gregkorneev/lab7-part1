#pragma once
#include <vector>

namespace algo {

// Последовательный поиск с «сторожем». Возвращает индекс или -1.
int sequential_search_sentinel(const std::vector<int>& a, int key);

} // namespace algo
