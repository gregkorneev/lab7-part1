#pragma once
#include <vector>
namespace simple {

struct Point {
    double x;
    double y;
    int    id;   // индекс в исходном массиве
};


struct CPResult {
    int i;
    int j;
    double dist;
};

// Полный перебор: O(n^2), максимально понятно.
CPResult closest_pair_bruteforce(const std::vector<Point>& pts);

// "Разделяй и властвуй": O(n log n), упрощённая и наглядная версия.
CPResult closest_pair_divide_conquer(std::vector<Point> pts);

}
