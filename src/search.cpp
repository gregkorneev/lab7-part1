#include "search.h"
#include <vector>

namespace algo {

int sequential_search_sentinel(const std::vector<int>& src, int key) {
    if (src.empty()) return -1;
    std::vector<int> a = src;
    a.push_back(key); // сторож
    int i = 0;
    while (a[i] != key) ++i;
    return (i < (int)src.size()) ? i : -1;
}

} // namespace algo
