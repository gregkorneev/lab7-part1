#include "../include/searching_algorithms.h"
#include <vector>

namespace simple {

int sequential_search_sentinel(const std::vector<int>& src, int key) {
    if (src.empty()) return -1;

    std::vector<int> a = src; // простая копия, чтобы не портить исходник
    a.push_back(key);         // сторож

    int i = 0;
    while (a[i] != key) {     // точно завершится: в конце лежит key
        ++i;
    }
    if (i < (int)src.size())  // нашли до сторожа — значит, был в исходном
        return i;
    return -1;
}

}
