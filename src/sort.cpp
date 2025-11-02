cat > src/sort.cpp <<'EOF'
#include "sort.h"
#include <algorithm>
#include <vector>

namespace algo {

void selection_sort(std::vector<int>& a) {
    int n = (int)a.size();
    for (int i = 0; i < n - 1; ++i) {
        int minj = i;
        for (int j = i + 1; j < n; ++j)
            if (a[j] < a[minj]) minj = j;
        std::swap(a[i], a[minj]);
    }
}

void bubble_sort(std::vector<int>& a) {
    int n = (int)a.size();
    bool swapped = true;
    for (int i = 0; i < n - 1 && swapped; ++i) {
        swapped = false;
        for (int j = 0; j < n - 1 - i; ++j)
            if (a[j] > a[j + 1]) {
                std::swap(a[j], a[j + 1]);
                swapped = true;
            }
    }
}

static void merge_range(std::vector<int>& a, int l, int m, int r, std::vector<int>& tmp) {
    int i = l, j = m, k = l;
    while (i < m && j < r) tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i < m) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];
    for (int t = l; t < r; ++t) a[t] = tmp[t];
}

static void merge_sort_rec(std::vector<int>& a, int l, int r, std::vector<int>& tmp) {
    if (r - l <= 1) return;
    int m = l + (r - l) / 2;
    merge_sort_rec(a, l, m, tmp);
    merge_sort_rec(a, m, r, tmp);
    if (a[m - 1] <= a[m]) return;
    merge_range(a, l, m, r, tmp);
}

void merge_sort(std::vector<int>& a) {
    std::vector<int> tmp(a.size());
    merge_sort_rec(a, 0, (int)a.size(), tmp);
}

} // namespace algo
EOF
