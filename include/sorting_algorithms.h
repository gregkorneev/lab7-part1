#pragma once
#include <vector>

// БАЗОВЫЕ СОРТИРОВКИ ПО ВОЗРАСТАНИЮ ДЛЯ std::vector<int>.
// Никаких шаблонов и лишних зависимостей: только int.

namespace simple {

// Сортировка выбором: O(n^2)
void selection_sort(std::vector<int>& a);

// Сортировка пузырьком: O(n^2)
void bubble_sort(std::vector<int>& a);

// Сортировка слиянием: O(n log n)
void merge_sort(std::vector<int>& a);

} // namespace simple
