/*
 * Три простые сортировки: выбором, пузырьком и слиянием.
 * Без классов, лямбд и продвинутых приёмов.
 */
#include "../include/sorting_algorithms.h"
#include <vector>
#include <algorithm> // std::swap

namespace simple {

// Сортировка выбором: O(n^2)
void selection_sort(std::vector<int>& a) {
    int n = (int)a.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_pos = i;
        for (int j = i + 1; j < n; ++j) {
            if (a[j] < a[min_pos]) min_pos = j;
        }
        if (min_pos != i) {
            std::swap(a[i], a[min_pos]);
        }
    }
}

// Сортировка пузырьком: O(n^2)
void bubble_sort(std::vector<int>& a) {
    int n = (int)a.size();
    for (int pass = 0; pass < n - 1; ++pass) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - pass; ++j) {
            if (a[j] > a[j + 1]) {
                std::swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // уже отсортировано
    }
}

// Вспомогательное слияние двух отсортированных частей [l..m) и [m..r)
static void merge_range(std::vector<int>& a, int l, int m, int r, std::vector<int>& tmp) {
    int i = l, j = m, k = l;
    while (i < m && j < r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else               tmp[k++] = a[j++];
    }
    while (i < m) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];
    for (int t = l; t < r; ++t) a[t] = tmp[t];
}

// Рекурсивная часть сортировки слиянием
static void merge_sort_rec(std::vector<int>& a, int l, int r, std::vector<int>& tmp) {
    if (r - l <= 1) return;
    int m = l + (r - l) / 2;
    merge_sort_rec(a, l, m, tmp);
    merge_sort_rec(a, m, r, tmp);
    if (a[m - 1] <= a[m]) return; // уже упорядочено
    merge_range(a, l, m, r, tmp);
}

// Сортировка слиянием: O(n log n)
void merge_sort(std::vector<int>& a) {
    std::vector<int> tmp(a.size());
    merge_sort_rec(a, 0, (int)a.size(), tmp);
}

} // namespace simple
