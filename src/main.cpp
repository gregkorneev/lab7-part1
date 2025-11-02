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
    setlocale(LC_ALL, ""); // для корректного вывода кириллицы в терминале
    std::cout << "=== Лабораторная работа 7–1: Сортировка и поиск ближайших точек ===\n\n";

    // ---- Сортировки ----
    const int N = 5000;
    auto base = make_array(N);

    auto test_sort = [&](const char* name, void(*fn)(std::vector<int>&), int sampleN) {
        auto a = base;
        auto t0 = Clock::now();
        fn(a);
        auto t1 = Clock::now();

        bool ok = std::is_sorted(a.begin(), a.end());
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

        std::cout << std::left << std::setw(14) << name
                  << " | отсортировано=" << (ok ? "да" : "НЕТ")
                  << " | время=" << ms << " мс\n";

        std::cout << "  первые элементы: ";
        for (int i = 0; i < sampleN && i < (int)a.size(); ++i) std::cout << a[i] << ' ';
        std::cout << "\n\n";
        return a;
    };

    std::cout << "-- СОРТИРОВКИ --\n";
    auto a1 = test_sort("Выбором", algo::selection_sort, 5);
    auto a2 = test_sort("Пузырьком", algo::bubble_sort, 5);
    auto a3 = test_sort("Слиянием", algo::merge_sort, 5);

    std::cout << "Результаты совпадают (слияние vs выбор): "
              << std::boolalpha << (a1 == a3) << "\n";
    std::cout << "Результаты совпадают (пузырёк vs выбор): "
              << std::boolalpha << (a1 == a2) << "\n\n";

    // ---- Последовательный поиск ----
    int key_present = base[N / 3];
    int key_absent = 123456789;

    int pos1 = algo::sequential_search_sentinel(base, key_present);
    int pos2 = algo::sequential_search_sentinel(base, key_absent);

    std::cout << "-- ПОСЛЕДОВАТЕЛЬНЫЙ ПОИСК (со сторожем) --\n";
    std::cout << "Индекс найденного элемента: " << pos1 << "\n";
    std::cout << "Результат при отсутствии элемента: " << pos2 << "\n\n";

    // ---- Поиск ближайшей пары точек ----
    const int M = 4000;
    auto pts = make_points(M);

    std::cout << "-- ПОИСК БЛИЖАЙШЕЙ ПАРЫ ТОЧЕК --\n";

    auto t0 = Clock::now();
    auto bf = algo::closest_pair_bruteforce(pts);
    auto t1 = Clock::now();
    auto dc = algo::closest_pair_divide_conquer(pts);
    auto t2 = Clock::now();

    auto ms_bf = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto ms_dc = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "Метод перебора:      i=" << bf.i << " j=" << bf.j
              << "  расстояние=" << bf.dist << "  время=" << ms_bf << " мс\n";
    std::cout << "Метод декомпозиции:  i=" << dc.i << " j=" << dc.j
              << "  расстояние=" << dc.dist << "  время=" << ms_dc << " мс\n";
    std::cout << "Результаты совпадают (погрешность 1e-9): "
              << (std::abs(bf.dist - dc.dist) < 1e-9 ? "да" : "нет") << "\n";

    std::cout << "\n=== Завершено успешно ===\n";
    return 0;
}
