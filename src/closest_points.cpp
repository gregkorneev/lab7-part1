#include "closest_points.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

namespace algo {

static inline double dist2(const Point& a, const Point& b) {
    const double dx = a.x - b.x;
    const double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// --- 1) Полный перебор O(n^2)
ClosestPairResult closest_pair_bruteforce(const std::vector<Point>& pts) {
    const int n = static_cast<int>(pts.size());
    ClosestPairResult res{ -1, -1, std::numeric_limits<double>::infinity() };
    if (n < 2) return res;

    double best2 = std::numeric_limits<double>::infinity();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double d2 = dist2(pts[i], pts[j]);
            if (d2 < best2) {
                best2 = d2;
                res = { pts[i].id, pts[j].id, std::sqrt(d2) };
            }
        }
    }
    return res;
}

// --- 2) Разделяй и властвуй O(n log n)
// Вспомогательная рекурсивная функция: принимает точки, отсортированные по X и по Y
static ClosestPairResult solve_rec(std::vector<Point>& byX, std::vector<Point>& byY) {
    const int n = static_cast<int>(byX.size());
    if (n <= 3) {
        return closest_pair_bruteforce(byX);
    }

    const int mid = n / 2;
    const double midx = byX[mid].x;

    std::vector<Point> leftX(byX.begin(), byX.begin() + mid);
    std::vector<Point> rightX(byX.begin() + mid, byX.end());

    std::vector<Point> leftY;  leftY.reserve(mid);
    std::vector<Point> rightY; rightY.reserve(n - mid);
    for (const auto& p : byY) {
        (p.x < midx ? leftY : rightY).push_back(p);
    }

    auto L = solve_rec(leftX, leftY);
    auto R = solve_rec(rightX, rightY);
    ClosestPairResult best = (L.dist < R.dist ? L : R);
    double d = best.dist;

    // Формируем «полосу» ширины 2d по X, отсортированную по Y (у нас уже byY)
    std::vector<Point> strip;
    strip.reserve(n);
    for (const auto& p : byY) {
        if (std::abs(p.x - midx) < d) strip.push_back(p);
    }

    // В полосе достаточно проверять не более 7 следующих по Y точек
    const int m = static_cast<int>(strip.size());
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m && (strip[j].y - strip[i].y) < d; ++j) {
            double d2 = dist2(strip[i], strip[j]);
            if (d2 < d * d) {
                d = std::sqrt(d2);
                best = { strip[i].id, strip[j].id, d };
            }
        }
    }
    return best;
}

ClosestPairResult closest_pair_divide_conquer(std::vector<Point> pts) {
    const int n = static_cast<int>(pts.size());
    if (n < 2) return { -1, -1, std::numeric_limits<double>::infinity() };

    // Проставим id на всякий случай
    for (int i = 0; i < n; ++i) pts[i].id = i;

    std::vector<Point> byX = pts;
    std::sort(byX.begin(), byX.end(), [](const Point& a, const Point& b) {
        if (a.x == b.x) return a.y < b.y;
        return a.x < b.x;
    });

    std::vector<Point> byY = pts;
    std::sort(byY.begin(), byY.end(), [](const Point& a, const Point& b) {
        if (a.y == b.y) return a.x < b.x;
        return a.y < b.y;
    });

    return solve_rec(byX, byY);
}

} // namespace algo
