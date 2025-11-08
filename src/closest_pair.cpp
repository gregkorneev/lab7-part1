/*
 * Ближайшая пара точек: простой перебор и базовая версия "разделяй и властвуй".
 * Без лямбд: отдельные функции-компараторы cmpX/cmpY.
 */
#include "../include/closest_pair.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

namespace simple {

static inline double dist2(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Полный перебор: O(n^2)
CPResult closest_pair_bruteforce(const std::vector<Point>& pts) {
    CPResult res;
    res.i = -1; res.j = -1; res.dist = std::numeric_limits<double>::infinity();

    int n = (int)pts.size();
    if (n < 2) return res;

    double best2 = std::numeric_limits<double>::infinity();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double d2 = dist2(pts[i], pts[j]);
            if (d2 < best2) {
                best2 = d2;
                res.i = pts[i].id;
                res.j = pts[j].id;
                res.dist = std::sqrt(d2);
            }
        }
    }
    return res;
}

// Компараторы для сортировки без лямбд
static bool cmpX(const Point& a, const Point& b) {
    if (a.x < b.x) return true;
    if (a.x > b.x) return false;
    return a.y < b.y;
}
static bool cmpY(const Point& a, const Point& b) {
    if (a.y < b.y) return true;
    if (a.y > b.y) return false;
    return a.x < b.x;
}

// Рекурсивная часть "разделяй и властвуй"
static CPResult solve_rec(std::vector<Point>& byX, std::vector<Point>& byY) {
    int n = (int)byX.size();
    if (n <= 3) {
        return closest_pair_bruteforce(byX);
    }

    int mid = n / 2;
    double midX = byX[mid].x;

    std::vector<Point> leftX(byX.begin(), byX.begin() + mid);
    std::vector<Point> rightX(byX.begin() + mid, byX.end());

    std::vector<Point> leftY;  leftY.reserve(mid);
    std::vector<Point> rightY; rightY.reserve(n - mid);
    for (int k = 0; k < (int)byY.size(); ++k) {
        const Point& p = byY[k];
        if (p.x < midX) leftY.push_back(p);
        else            rightY.push_back(p);
    }

    CPResult L = solve_rec(leftX, leftY);
    CPResult R = solve_rec(rightX, rightY);
    CPResult best = (L.dist < R.dist) ? L : R;
    double d = best.dist;

    // Полоса ширины 2d вокруг midX (byY уже отсортирован по Y)
    std::vector<Point> strip;
    strip.reserve(n);
    for (int k = 0; k < (int)byY.size(); ++k) {
        const Point& p = byY[k];
        if (std::fabs(p.x - midX) < d) strip.push_back(p);
    }

    // Проверяем соседей по Y
    int m = (int)strip.size();
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            if ((strip[j].y - strip[i].y) >= d) break;
            double d2 = dist2(strip[i], strip[j]);
            if (d2 < d * d) {
                d = std::sqrt(d2);
                best.i = strip[i].id;
                best.j = strip[j].id;
                best.dist = d;
            }
        }
    }
    return best;
}

// Обёртка: подготовка данных
CPResult closest_pair_divide_conquer(std::vector<Point> pts) {
    CPResult res;
    res.i = -1; res.j = -1; res.dist = std::numeric_limits<double>::infinity();

    int n = (int)pts.size();
    if (n < 2) return res;

    for (int i = 0; i < n; ++i) pts[i].id = i;

    std::vector<Point> byX = pts;
    std::vector<Point> byY = pts;
    std::sort(byX.begin(), byX.end(), cmpX);
    std::sort(byY.begin(), byY.end(), cmpY);

    return solve_rec(byX, byY);
}

} // namespace simple
