/*
 * Поиски: последовательный со "сторожем" и бинарный (на отсортированном массиве).
 */
#include "../include/searching_algorithms.h"
#include <vector>

namespace simple {

// Последовательный поиск со "сторожем"
int sequential_search_sentinel(const std::vector<int>& src, int key) {
    if (src.empty()) return -1;

    std::vector<int> a = src; // копия, чтобы не портить исходник
    a.push_back(key);         // сторож

    int i = 0;
    while (a[i] != key) ++i;

    if (i < (int)src.size()) return i; // нашли до сторожа
    return -1;                         // не нашли
}

// Бинарный поиск (массив ДОЛЖЕН быть отсортирован по возрастанию)
int binary_search(const std::vector<int>& a, int key) {
    int L = 0;
    int R = (int)a.size() - 1;
    while (L <= R) {
        int M = L + (R - L) / 2;
        if (a[M] == key) return M;
        if (a[M] < key)  L = M + 1;
        else             R = M - 1;
    }
    return -1;
}

} // namespace simple
