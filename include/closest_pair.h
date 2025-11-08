#pragma once
#include <vector>

// ПОИСК БЛИЖАЙШЕЙ ПАРЫ ТОЧЕК.
// Даны две реализации: полный перебор и "разделяй и властвуй".

namespace simple {

// Простейшая точка на плоскости с исходным индексом.
struct Point {
    double x;
    double y;
    int    id;   // индекс в исходном массиве
};

// Результат: индексы точек в исходном массиве и расстояние между ними.
struct CPResult {
    int i;
    int j;
    double dist;
};

// Полный перебор: O(n^2), максимально понятно.
CPResult closest_pair_bruteforce(const std::vector<Point>& pts);

// "Разделяй и властвуй": O(n log n), упрощённая и наглядная версия.
CPResult closest_pair_divide_conquer(std::vector<Point> pts);

} // namespace simple
