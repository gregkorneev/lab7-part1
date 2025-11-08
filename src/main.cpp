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

// ---------------- ВСПОМОГАТЕЛЬНЫЕ ГЕНЕРАТОРЫ ----------------

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

static std::vector<int> make_avg_case(int n, int seed) {
    return make_array(n, seed);
}

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

// ---- время в миллисекунды ----
static double ms_since(std::clock_t t0, std::clock_t t1) {
    return 1000.0 * (t1 - t0) / CLOCKS_PER_SEC;
}

// Измерить время для сортировочной функции вида void f(std::vector<int>&)
static double measure_sort(void (*sort_fn)(std::vector<int>&), std::vector<int>& a) {
    std::clock_t t0 = std::clock();
    sort_fn(a);
    std::clock_t t1 = std::clock();
    return ms_since(t0, t1);
}
static int binary_search_basic(const std::vector<int>& a, int key) {
    int L = 0, R = (int)a.size() - 1;
    while (L <= R) {
        int M = L + (R - L) / 2;
        if (a[M] == key) return M;
        if (a[M] < key)  L = M + 1;
        else             R = M - 1;
    }
    return -1;
}

// Короткий вывод статуса сортировки
static void print_sort_status(const char* name, const std::vector<int>& v, double ms) {
    std::cout << name << " | отсортировано=" << (std::is_sorted(v.begin(), v.end()) ? "да" : "нет")
              << " | время=" << ms << " мс\n";
}

//MAIN
int main() {
    std::cout << "=== Лабораторная 7–1: Сортировки, поиски, ближайшая пара ===\n\n";

    // ================= СОРТИРОВКИ: базовый эксперимент =================
    const int N = 2000;
    std::vector<int> base = make_array(N, 42);

    std::vector<int> a_sel = base;
    std::vector<int> a_bub = base;
    std::vector<int> a_mer = base;

    double ms_sel = measure_sort(simple::selection_sort, a_sel);
    double ms_bub = measure_sort(simple::bubble_sort,    a_bub);
    double ms_mer = measure_sort(simple::merge_sort,     a_mer);

    std::cout << "-- СОРТИРОВКИ --\n";
    print_sort_status("Выбором  ", a_sel, ms_sel);
    print_sort_status("Пузырьком", a_bub, ms_bub);
    print_sort_status("Слиянием ", a_mer, ms_mer);

    bool eq_mer_sel = (a_mer == a_sel);
    bool eq_bub_sel = (a_bub == a_sel);
    std::cout << "Совпадение (слияние vs выбор): " << (eq_mer_sel ? "да" : "нет") << "\n";
    std::cout << "Совпадение (пузырёк  vs выбор): " << (eq_bub_sel ? "да" : "нет") << "\n\n";

    // CSV: базовые времена сортировок
    csvout::save_sorting_csv("sorting.csv", ms_sel, ms_bub, ms_mer, eq_mer_sel, eq_bub_sel);

    // ================= ПОИСКИ: последовательный + бинарный =================
    int key_present = base[N / 3];  // гарантированно есть
    int key_absent  = 123456789;    // гарантированно нет

    int pos_seq_ok   = simple::sequential_search_sentinel(base, key_present);
    int pos_seq_miss = simple::sequential_search_sentinel(base, key_absent);

    std::vector<int> base_sorted = base;
    simple::merge_sort(base_sorted);
    int pos_bin_ok = binary_search_basic(base_sorted, key_present);

    std::cout << "-- ПОИСК --\n";
    std::cout << "Последовательный: есть=" << pos_seq_ok << ", нет=" << pos_seq_miss << "\n";
    std::cout << "Бинарный (на отсорт.) : есть=" << pos_bin_ok << "\n\n";

    // CSV: поиск (последовательный)
    csvout::save_search_csv("search.csv", pos_seq_ok, pos_seq_miss);

    // ================= БЛИЖАЙШАЯ ПАРА ТОЧЕК =================
    const int M = 2000;
    std::vector<simple::Point> pts = make_points(M, 123);

    std::clock_t t0 = std::clock();
    simple::CPResult bf = simple::closest_pair_bruteforce(pts);
    std::clock_t t1 = std::clock();
    double ms_bf = ms_since(t0, t1);

    t0 = std::clock();
    simple::CPResult dc = simple::closest_pair_divide_conquer(pts);
    t1 = std::clock();
    double ms_dc = ms_since(t0, t1);

    std::cout << "-- БЛИЖАЙШАЯ ПАРА --\n";
    std::cout << "Перебор: i=" << bf.i << " j=" << bf.j << " dist=" << bf.dist << " t=" << ms_bf << " мс\n";
    std::cout << "D&C   : i=" << dc.i << " j=" << dc.j << " dist=" << dc.dist << " t=" << ms_dc << " мс\n";
    std::cout << "Совпадение расстояний (1e-9): "
              << (std::fabs(bf.dist - dc.dist) < 1e-9 ? "да" : "нет") << "\n\n";
    // CSV: ближайшая пара
    csvout::save_closest_csv("closest_pair.csv", bf, ms_bf, dc, ms_dc);

    // ================= СЛУЧАИ ДЛЯ СОРТИРОВОК (Л/С/Х) =================
    const int N_cases = 1000;
    std::vector<int> best  = make_best_case(N_cases);
    std::vector<int> avg   = make_avg_case (N_cases, 2025);
    std::vector<int> worst = make_worst_case(N_cases);

    std::vector<int> b1 = best,  b2 = best,  b3 = best;
    std::vector<int> m1 = avg,   m2 = avg,   m3 = avg;
    std::vector<int> w1 = worst, w2 = worst, w3 = worst;

    double t_sel_best  = measure_sort(simple::selection_sort, b1);
    double t_bub_best  = measure_sort(simple::bubble_sort,    b2);
    double t_mer_best  = measure_sort(simple::merge_sort,     b3);

    double t_sel_avg   = measure_sort(simple::selection_sort, m1);
    double t_bub_avg   = measure_sort(simple::bubble_sort,    m2);
    double t_mer_avg   = measure_sort(simple::merge_sort,     m3);

    double t_sel_worst = measure_sort(simple::selection_sort, w1);
    double t_bub_worst = measure_sort(simple::bubble_sort,    w2);
    double t_mer_worst = measure_sort(simple::merge_sort,     w3);

    csvout::save_sorting_cases_csv("sorting_cases.csv",
        t_sel_best,  t_bub_best,  t_mer_best,
        t_sel_avg,   t_bub_avg,   t_mer_avg,
        t_sel_worst, t_bub_worst, t_mer_worst);

    // ================= СЕРИИ ПО РАЗМЕРАМ (для графиков) =================
    {
        std::ofstream ofs("sorting_sizes.csv");
        ofs << "n;method;time_ms\n";
        const int sizes[] = {100, 300, 500, 1000, 2000, 5000};
        for (int i = 0; i < 6; ++i) {
            int n = sizes[i];
            std::vector<int> s = make_array(n, 777);
            std::vector<int> a = s, b = s, c = s;

            ofs << n << ";selection;" << measure_sort(simple::selection_sort, a) << "\n";
            ofs << n << ";bubble;"    << measure_sort(simple::bubble_sort,    b) << "\n";
            ofs << n << ";merge;"     << measure_sort(simple::merge_sort,     c) << "\n";
        }
    }
    {
        std::ofstream ofs("search_sizes.csv");
        ofs << "n;method;time_ms\n";
        const int sizes[] = {100, 300, 500, 1000, 2000, 5000};
        for (int i = 0; i < 6; ++i) {
            int n = sizes[i];
            std::vector<int> d = make_array(n, 888);
            int key = d[n / 2];

            // последовательный
            std::clock_t t0s = std::clock();
            (void)simple::sequential_search_sentinel(d, key);
            std::clock_t t1s = std::clock();
            ofs << n << ";sequential;" << ms_since(t0s, t1s) << "\n";

            // бинарный (на отсортированном массиве)
            std::vector<int> d_sorted = d;
            simple::merge_sort(d_sorted);
            t0s = std::clock();
            (void)binary_search_basic(d_sorted, key);
            t1s = std::clock();
            ofs << n << ";binary;" << ms_since(t0s, t1s) << "\n";
        }
    }

    std::cout << "CSV готовы: sorting.csv, sorting_cases.csv, sorting_sizes.csv,\n"
                 "           search.csv, search_sizes.csv, closest_pair.csv\n";
    std::cout << "Готово\n";
    return 0;
}
