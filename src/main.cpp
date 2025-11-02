#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <cmath>

#include "sort.h"
#include "search.h"
#include "closest_points.h"

using Clock = std::chrono::high_resolution_clock;

static std::vector<int> make_array(int n, int seed = 42) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(-100000, 100000);
    std::vector<int> a(n);
    for (int& v : a) v = dist(rng);
    return a;
}

static std::vector<algo::Point> make_points(int n, int seed = 123) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 10000.0);
    std::vector<algo::Point> p(n);
    for (int i = 0; i < n; ++i) p[i] = { dist(rng), dist(rng), i };
    return p;
}

int main() {
    std::cout << "=== Lab 7-1: Sorting & Closest Pair ===\n\n";

    // ---- Сортировки ----
    // Для демонстрации оставил небольшие размеры, чтобы не ждать долго.
    const int N = 5000;
    auto base = make_array(N);

    auto test_sort = [&](const char* name, void(*fn)(std::vector<int>&), int sampleN) {
        auto a = base; // копия
        auto t0 = Clock::now();
        fn(a);
        auto t1 = Clock::now();

        bool ok = std::is_sorted(a.begin(), a.end());
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

        std::cout << std::left << std::setw(14) << name
                  << " | sorted=" << (ok ? "yes" : "NO")
                  << " | time=" << ms << " ms\n";

        std::cout << "  first: ";
        for (int i = 0; i < sampleN && i < (int)a.size(); ++i) std::cout << a[i] << ' ';
        std::cout << "\n\n";
        return a;
    };

    std::cout << "-- Sorting --\n";
    auto a1 = test_sort("SelectionSort", algo::selection_sort, 5);
    auto a2 = test_sort("BubbleSort",    algo::bubble_sort,    5);
    auto a3 = test_sort("MergeSort",     algo::merge_sort,     5);

    std::cout << "Results equal (merge vs selection): "
              << std::boolalpha << (a1 == a3) << "\n";
    std::cout << "Results equal (bubble vs selection):  "
              << std::boolalpha << (a1 == a2) << "\n\n";

    // ---- Последовательный поиск (sentinel) ----
    int key_present = base[N / 3];
    int key_absent  = 123456789;

    int pos1 = algo::sequential_search_sentinel(base, key_present);
    int pos2 = algo::sequential_search_sentinel(base, key_absent);

    std::cout << "-- Sequential search (sentinel) --\n";
    std::cout << "key present index: " << pos1 << "\n";
    std::cout << "absent returns:    " << pos2 << "\n\n";

    // ---- Пара ближайших точек ----
    const int M = 4000; // увеличь/уменьши при необходимости
    auto pts = make_points(M);

    std::cout << "-- Closest pair of points --\n";
    auto t0 = Clock::now();
    auto bf  = algo::closest_pair_bruteforce(pts);
    auto t1 = Clock::now();
    auto dc  = algo::closest_pair_divide_conquer(pts);
    auto t2 = Clock::now();

    auto ms_bf = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto ms_dc = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "BruteForce:  i=" << bf.i << " j=" << bf.j
              << " dist=" << bf.dist << " time=" << ms_bf << " ms\n";
    std::cout << "Divide&Conq: i=" << dc.i << " j=" << dc.j
              << " dist=" << dc.dist << " time=" << ms_dc << " ms\n";
    std::cout << "Distances equal (eps=1e-9): "
              << (std::abs(bf.dist - dc.dist) < 1e-9 ? "yes" : "no") << "\n";

    return 0;
}
