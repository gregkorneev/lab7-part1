cat > include/closest_points.h <<'EOF'
#pragma once
#include <vector>
#include <utility>
#include <limits>

namespace algo {

struct Point {
    double x{}, y{};
    int id{};
};

struct ClosestPairResult {
    int i{-1}, j{-1};
    double dist{};
};

ClosestPairResult closest_pair_bruteforce(const std::vector<Point>& pts);
ClosestPairResult closest_pair_divide_conquer(std::vector<Point> pts);

} // namespace algo
EOF
