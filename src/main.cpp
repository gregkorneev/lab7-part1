#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include "../include/sorting_algorithms.h"
#include "../include/searching_algorithms.h"
#include "../include/closest_pair.h"
static std::vector<int> make_array(int n, int seed) {
    std::srand(seed);
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = std::rand() % 200001 - 100000;
    }
    return a;
}
static std::vector<simple::Point> make_points(int n, int seed) {
    std::srand(seed);
    std::vector<simple::Point> p(n);
    for (int i = 0; i < n; ++i) {
        p[i].x = (std::rand() % 10000);
        p[i].y = (std::rand() % 10000);
        p[i].id = i;
    }
    return p;
}
static double ms_since(std::clock_t t0, std::clock_t t1) {
    return 1000.0 * (t1 - t0) / CLOCKS_PER_SEC;
}
int main() {
    std::cout << "=== Лабораторная работа 7–1: Сортировка и ближайшая пара ===\n\n";

    // ---------------- СОРТИРОВКИ ----------------
    const int N = 2000;
    std::vector<int> base = make_array(N, /*seed=*/42);

    std::vector<int> a_sel = base;
    std::vector<int> a_bub = base;
    std::vector<int> a_mer = base;

    std::clock_t t0, t1;

    t0 = std::clock(); simple::selection_sort(a_sel); t1 = std::clock();
    double ms_sel = ms_since(t0, t1);

    t0 = std::clock(); simple::bubble_sort(a_bub);   t1 = std::clock();
    double ms_bub = ms_since(t0, t1);

    t0 = std::clock(); simple::merge_sort(a_mer);    t1 = std::clock();
    double ms_mer = ms_since(t0, t1);

    std::cout << "-- СОРТИРОВКИ --\n";
    std::cout << "Выбором  | отсортировано=" << (std::is_sorted(a_sel.begin(), a_sel.end()) ? "да" : "нет")
              << " | время=" << ms_sel << " мс\n";
    std::cout << "Пузырьком| отсортировано=" << (std::is_sorted(a_bub.begin(), a_bub.end()) ? "да" : "нет")
              << " | время=" << ms_bub << " мс\n";
    std::cout << "Слиянием | отсортировано=" << (std::is_sorted(a_mer.begin(), a_mer.end()) ? "да" : "нет")
              << " | время=" << ms_mer << " мс\n";
    std::cout << "Результаты совпадают (слияние vs выбор): "
              << ((a_mer == a_sel) ? "да" : "нет") << "\n";
    std::cout << "Результаты совпадают (пузырёк vs выбор): "
              << ((a_bub == a_sel) ? "да" : "нет") << "\n\n";

    // ---------------- ПОСЛЕДОВАТЕЛЬНЫЙ ПОИСК ----------------
    int key_present = base[N / 3];   // точно существует
    int key_absent  = 123456789;     // точно не существует

    int pos1 = simple::sequential_search_sentinel(base, key_present);
    int pos2 = simple::sequential_search_sentinel(base, key_absent);

    std::cout << "-- ПОСЛЕДОВАТЕЛЬНЫЙ ПОИСК (со сторожем) --\n";
    std::cout << "Индекс найденного элемента: " << pos1 << "\n";
    std::cout << "Результат при отсутствии элемента: " << pos2 << "\n\n";

    // ---------------- БЛИЖАЙШАЯ ПАРА ТОЧЕК ----------------
    const int M = 2000;
    std::vector<simple::Point> pts = make_points(M, /*seed=*/123);

    t0 = std::clock(); simple::CPResult bf = simple::closest_pair_bruteforce(pts);        t1 = std::clock();
    double ms_bf = ms_since(t0, t1);

    t0 = std::clock(); simple::CPResult dc = simple::closest_pair_divide_conquer(pts);    t1 = std::clock();
    double ms_dc = ms_since(t0, t1);

    std::cout << "-- ПОИСК БЛИЖАЙШЕЙ ПАРЫ ТОЧЕК --\n";
    std::cout << "Метод перебора     : i=" << bf.i << " j=" << bf.j
              << " расстояние=" << bf.dist << " время=" << ms_bf << " мс\n";
    std::cout << "Метод декомпозиции : i=" << dc.i << " j=" << dc.j
              << " расстояние=" << dc.dist << " время=" << ms_dc << " мс\n";
    std::cout << "Результаты совпадают (погрешность 1e-9): "
              << (std::fabs(bf.dist - dc.dist) < 1e-9 ? "да" : "нет") << "\n";

    std::cout << "\n=== Завершено успешно ===\n";
    return 0;
}
