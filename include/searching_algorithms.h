#pragma once
#include <vector>

// ПРОСТОЙ ПОСЛЕДОВАТЕЛЬНЫЙ ПОИСК СО "СТОРОЖЕМ" ДЛЯ std::vector<int>.

namespace simple {

// Возвращает индекс найденного элемента или -1, если элемента нет.
int sequential_search_sentinel(const std::vector<int>& a, int key);

} // namespace simple
