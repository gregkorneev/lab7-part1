/*
 * ЛР 7–1: Сортировки, линейный (со сторожем) и бинарный поиск, ближайшая пара точек.
 * Всё максимально просто. Время считаем через std::clock().
 *
 * CSV:
 *   sorting.csv        — базовые времена сортировок
 *   sorting_cases.csv  — лучший/средний/худший случаи
 *   sorting_sizes.csv  — серии по размерам для сортировок
 *   search.csv         — результаты последовательного поиска (present/absent)
 *   search_sizes.csv   — серии по размерам для sequential и binary
 *   closest_pair.csv   — сравнение brute force и divide&conquer
 */
#include <iostream>
#include <vector>
#include <cstdlib>    // rand, srand
#include <ctime>      // clock, CLOCKS_PER_SEC
#include <algorithm>  // std::is_sorted
#include <cmath>      // std::fabs
#include <fstream>    // std::ofstream

#include "../include/sorting_algorithms.h"
#include "../include/searching_algorithms.h"
#include "../include/closest_pair.h"
#include "../include/results_csv.h"

// ---------- простые генераторы ----------
static std::vector<int> make_array(int n, int seed) {
    std::srand(seed);
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = std::rand() % 200001 - 100000;
    return a;
}
static std::vector<int> make_best_case(int n) {
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = i;
    return a;
}
static std::vector<int> make_avg_case(int n, int seed) { return make_array(n, seed); }
static std::vector<int> make_worst_case(int n) {
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = n - 1 - i;
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

// ---------- ЛОКАЛЬНЫЙ БИНАРНЫЙ ПОИСК (чтобы не трогать заголовки) ----------
static int binary_search_basic(const std::vector<int>& a, int key) {
    int L = 0;
    int R = (int)a.size() - 1;
    while (L <= R) {
        int M = L + (R - L) / 2;
        if (a[M] == key) return M;
        if (a[M] < key)  L = M + 1;
        else             R = M - 1;
    }
    return -1;
}

// -------------------- main --------------------
int main() {
    std::cout << "Лабораторная 7–1: Сортировки, поиски, ближайшая пара n\n";

    std::clock_t t0, t1;

    // ---- Базовый эксперимент: сортировки ----
    const int N = 2000;
    std::vector<int> base = make_array(N, 42);
    std::vector<int> a1 = base, a2 = base, a3 = base;

    t0 = std::clock(); simple::selection_sort(a1); t1 = std::clock(); double ms_sel = ms_since(t0,t1);
    t0 = std::clock(); simple::bubble_sort(a2);    t1 = std::clock(); double ms_bub = ms_since(t0,t1);
    t0 = std::clock(); simple::merge_sort(a3);     t1 = std::clock(); double ms_mer = ms_since(t0,t1);

    std::cout << "-- СОРТИРОВКИ --\n";
    std::cout << "Выбором  | отсортировано=" << (std::is_sorted(a1.begin(), a1.end()) ? "да" : "нет")
              << " | время=" << ms_sel << " мс\n";
    std::cout << "Пузырьком| отсортировано=" << (std::is_sorted(a2.begin(), a2.end()) ? "да" : "нет")
              << " | время=" << ms_bub << " мс\n";
    std::cout << "Слиянием | отсортировано=" << (std::is_sorted(a3.begin(), a3.end()) ? "да" : "нет")
              << " | время=" << ms_mer << " мс\n";
    bool eq_mer_sel = (a3 == a1);
    bool eq_bub_sel = (a2 == a1);
    std::cout << "Результаты совпадают (слияние vs выбор): " << (eq_mer_sel ? "да" : "нет") << "\n";
    std::cout << "Результаты совпадают (пузырёк vs выбор): " << (eq_bub_sel ? "да" : "нет") << "\n\n";

    csvout::save_sorting_csv("sorting.csv", ms_sel, ms_bub, ms_mer, eq_mer_sel, eq_bub_sel);

    // ---- Поиск: последовательный (со сторожем) + бинарный ----
    int key_present = base[N/3];
    int key_absent  = 123456789;

    int pos_seq_ok   = simple::sequential_search_sentinel(base, key_present);
    int pos_seq_miss = simple::sequential_search_sentinel(base, key_absent);

    // для бинарного нужен отсортированный массив
    std::vector<int> base_sorted = base;
    simple::merge_sort(base_sorted);
    int pos_bin_ok = binary_search_basic(base_sorted, key_present);

    std::cout << "-- ПОИСК --\n";
    std::cout << "Последовательный (есть): " << pos_seq_ok   << "\n";
    std::cout << "Последовательный (нет) : " << pos_seq_miss << "\n";
    std::cout << "Бинарный (есть на сорт.) : " << pos_bin_ok << "\n\n";

    csvout::save_search_csv("search.csv", pos_seq_ok, pos_seq_miss);

    // ---- Ближайшая пара точек ----
    const int M = 2000;
    std::vector<simple::Point> pts = make_points(M, 123);

    t0 = std::clock(); simple::CPResult bf = simple::closest_pair_bruteforce(pts);         t1 = std::clock();
    double ms_bf = ms_since(t0,t1);
    t0 = std::clock(); simple::CPResult dc = simple::closest_pair_divide_conquer(pts);     t1 = std::clock();
    double ms_dc = ms_since(t0,t1);

    std::cout << "-- БЛИЖАЙШЕЙ ПАРЫ --\n";
    std::cout << "Перебор      : i=" << bf.i << " j=" << bf.j
              << " расстояние=" << bf.dist << " время=" << ms_bf << " мс\n";
    std::cout << "Декомпозиция : i=" << dc.i << " j=" << dc.j
              << " расстояние=" << dc.dist << " время=" << ms_dc << " мс\n";
    std::cout << "Совпадение расстояний (1e-9): "
              << (std::fabs(bf.dist - dc.dist) < 1e-9 ? "да" : "нет") << "\n\n";

    csvout::save_closest_csv("closest_pair.csv", bf, ms_bf, dc, ms_dc);

    // ---- Лучший/Средний/Худший случаи для сортировок ----
    const int N_cases = 1000;
    std::vector<int> best  = make_best_case(N_cases);
    std::vector<int> avg   = make_avg_case (N_cases, 2025);
    std::vector<int> worst = make_worst_case(N_cases);

    std::vector<int> b1 = best,  b2 = best,  b3 = best;
    std::vector<int> m1 = avg,   m2 = avg,   m3 = avg;
    std::vector<int> w1 = worst, w2 = worst, w3 = worst;

    t0 = std::clock(); simple::selection_sort(b1); t1 = std::clock(); double t_sel_best  = ms_since(t0,t1);
    t0 = std::clock(); simple::bubble_sort(b2);    t1 = std::clock(); double t_bub_best  = ms_since(t0,t1);
    t0 = std::clock(); simple::merge_sort(b3);     t1 = std::clock(); double t_mer_best  = ms_since(t0,t1);

    t0 = std::clock(); simple::selection_sort(m1); t1 = std::clock(); double t_sel_avg   = ms_since(t0,t1);
    t0 = std::clock(); simple::bubble_sort(m2);    t1 = std::clock(); double t_bub_avg   = ms_since(t0,t1);
    t0 = std::clock(); simple::merge_sort(m3);     t1 = std::clock(); double t_mer_avg   = ms_since(t0,t1);

    t0 = std::clock(); simple::selection_sort(w1); t1 = std::clock(); double t_sel_worst = ms_since(t0,t1);
    t0 = std::clock(); simple::bubble_sort(w2);    t1 = std::clock(); double t_bub_worst = ms_since(t0,t1);
    t0 = std::clock(); simple::merge_sort(w3);     t1 = std::clock(); double t_mer_worst = ms_since(t0,t1);

    csvout::save_sorting_cases_csv("sorting_cases.csv",
        t_sel_best,  t_bub_best,  t_mer_best,
        t_sel_avg,   t_bub_avg,   t_mer_avg,
        t_sel_worst, t_bub_worst, t_mer_worst);

    // ---- Серии по размерам (для графиков в отчёте) ----
    {
        std::ofstream ofs("sorting_sizes.csv");
        ofs << "n;method;time_ms\n";
        const int sizes[] = {100, 300, 500, 1000, 2000, 5000};
        for (int n_i = 0; n_i < 6; ++n_i) {
            int n = sizes[n_i];
            std::vector<int> s = make_array(n, 777);
            std::vector<int> a = s, b = s, c = s;

            t0 = std::clock(); simple::selection_sort(a); t1 = std::clock();
            ofs << n << ";selection;" << ms_since(t0,t1) << "\n";

            t0 = std::clock(); simple::bubble_sort(b);    t1 = std::clock();
            ofs << n << ";bubble;"    << ms_since(t0,t1) << "\n";

            t0 = std::clock(); simple::merge_sort(c);     t1 = std::clock();
            ofs << n << ";merge;"     << ms_since(t0,t1) << "\n";
        }
    }
    {
        std::ofstream ofs("search_sizes.csv");
        ofs << "n;method;time_ms\n";
        const int sizes[] = {100, 300, 500, 1000, 2000, 5000};
        for (int n_i = 0; n_i < 6; ++n_i) {
            int n = sizes[n_i];
            std::vector<int> d = make_array(n, 888);
            int key = d[n/2];

            t0 = std::clock(); (void)simple::sequential_search_sentinel(d, key); t1 = std::clock();
            ofs << n << ";sequential;" << ms_since(t0,t1) << "\n";

            std::vector<int> d_sorted = d;
            simple::merge_sort(d_sorted);
            t0 = std::clock(); (void)binary_search_basic(d_sorted, key);         t1 = std::clock();
            ofs << n << ";binary;"     << ms_since(t0,t1) << "\n";
        }
    }

    std::cout << "Созданы CSV: sorting.csv, sorting_cases.csv, sorting_sizes.csv,\n"
                 "             search.csv, search_sizes.csv, closest_pair.csv\n";
    std::cout << "\n=== Завершено успешно ===\n";
    return 0;
}
