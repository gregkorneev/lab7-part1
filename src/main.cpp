/*
 * ЛР 7–1: Сортировки, линейный поиск (со сторожем) и ближайшая пара точек.
 * Вариант максимально простой: std::vector, циклы и std::clock().
 * CSV-отчёты: sorting.csv, search.csv, closest_pair.csv, sorting_cases.csv.
 */

#include <iostream>
#include <vector>
#include <cstdlib>    // rand, srand
#include <ctime>      // clock, time, CLOCKS_PER_SEC
#include <algorithm>  // std::is_sorted
#include <cmath>      // std::fabs

#include "../include/sorting_algorithms.h"
#include "../include/searching_algorithms.h"
#include "../include/closest_pair.h"
#include "../include/results_csv.h"

// ---------------- ВСПОМОГАТЕЛЬНЫЕ ГЕНЕРАТОРЫ ----------------

// Случайный массив в диапазоне [-100000; 100000]
static std::vector<int> make_array(int n, int seed) {
    std::srand(seed);
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = std::rand() % 200001 - 100000;
    return a;
}

// Лучший случай для сортировок: уже отсортирован по возрастанию
static std::vector<int> make_best_case(int n) {
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = i;
    return a;
}

// Средний случай: случайные значения
static std::vector<int> make_avg_case(int n, int seed) {
    return make_array(n, seed);
}

// Худший случай: строго убывающий
static std::vector<int> make_worst_case(int n) {
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = n - 1 - i;
    return a;
}

// Точки на плоскости (координаты ~ [0..9999])
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

// Перевод "тиков" процессора в миллисекунды
static double ms_since(std::clock_t t0, std::clock_t t1) {
    return 1000.0 * (t1 - t0) / CLOCKS_PER_SEC;
}

// ------------------------- MAIN -------------------------
int main() {
    std::cout << "=== Лабораторная работа 7–1: Сортировка и ближайшая пара ===\n\n";

    std::clock_t t0, t1;

    // ---------------- СОРТИРОВКИ (базовый эксперимент) ----------------
    const int N = 2000; // небольшое N, чтобы программа работала быстро
    std::vector<int> base = make_array(N, /*seed=*/42);

    std::vector<int> a_sel = base;
    std::vector<int> a_bub = base;
    std::vector<int> a_mer = base;

    t0 = std::clock(); simple::selection_sort(a_sel); t1 = std::clock();
    double ms_sel = ms_since(t0, t1);

    t0 = std::clock(); simple::bubble_sort(a_bub);    t1 = std::clock();
    double ms_bub = ms_since(t0, t1);

    t0 = std::clock(); simple::merge_sort(a_mer);     t1 = std::clock();
    double ms_mer = ms_since(t0, t1);

    std::cout << "-- СОРТИРОВКИ --\n";
    std::cout << "Выбором  | отсортировано=" << (std::is_sorted(a_sel.begin(), a_sel.end()) ? "да" : "нет")
              << " | время=" << ms_sel << " мс\n";
    std::cout << "Пузырьком| отсортировано=" << (std::is_sorted(a_bub.begin(), a_bub.end()) ? "да" : "нет")
              << " | время=" << ms_bub << " мс\n";
    std::cout << "Слиянием | отсортировано=" << (std::is_sorted(a_mer.begin(), a_mer.end()) ? "да" : "нет")
              << " | время=" << ms_mer << " мс\n";

    bool eq_mer_sel = (a_mer == a_sel);
    bool eq_bub_sel = (a_bub == a_sel);
    std::cout << "Результаты совпадают (слияние vs выбор): "
              << (eq_mer_sel ? "да" : "нет") << "\n";
    std::cout << "Результаты совпадают (пузырёк vs выбор): "
              << (eq_bub_sel ? "да" : "нет") << "\n\n";

    // CSV: базовые времена сортировок
    csvout::save_sorting_csv("sorting.csv", ms_sel, ms_bub, ms_mer, eq_mer_sel, eq_bub_sel);

    // ---------------- ПОСЛЕДОВАТЕЛЬНЫЙ ПОИСК (со сторожем) ----------------
    int key_present = base[N / 3];   // точно есть
    int key_absent  = 123456789;     // точно нет

    int pos1 = simple::sequential_search_sentinel(base, key_present);
    int pos2 = simple::sequential_search_sentinel(base, key_absent);

    std::cout << "-- ПОСЛЕДОВАТЕЛЬНЫЙ ПОИСК (со сторожем) --\n";
    std::cout << "Индекс найденного элемента: " << pos1 << "\n";
    std::cout << "Результат при отсутствии элемента: " << pos2 << "\n\n";

    // CSV: поиск
    csvout::save_search_csv("search.csv", pos1, pos2);

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
              << (std::fabs(bf.dist - dc.dist) < 1e-9 ? "да" : "нет") << "\n\n";

    // CSV: ближайшая пара
    csvout::save_closest_csv("closest_pair.csv", bf, ms_bf, dc, ms_dc);

    // ---------------- СЛУЧАИ ДЛЯ СОРТИРОВОК (Лучший/Средний/Худший) ----------------
    const int N_cases = 1000; // как в твоём примере
    auto best  = make_best_case(N_cases);
    auto avg   = make_avg_case(N_cases, 2025);
    auto worst = make_worst_case(N_cases);

    auto b1 = best,  b2 = best,  b3 = best;
    auto m1 = avg,   m2 = avg,   m3 = avg;
    auto w1 = worst, w2 = worst, w3 = worst;

    t0 = std::clock(); simple::selection_sort(b1); t1 = std::clock(); double t_sel_best  = ms_since(t0, t1);
    t0 = std::clock(); simple::bubble_sort(b2);    t1 = std::clock(); double t_bub_best  = ms_since(t0, t1);
    t0 = std::clock(); simple::merge_sort(b3);     t1 = std::clock(); double t_mer_best  = ms_since(t0, t1);

    t0 = std::clock(); simple::selection_sort(m1); t1 = std::clock(); double t_sel_avg   = ms_since(t0, t1);
    t0 = std::clock(); simple::bubble_sort(m2);    t1 = std::clock(); double t_bub_avg   = ms_since(t0, t1);
    t0 = std::clock(); simple::merge_sort(m3);     t1 = std::clock(); double t_mer_avg   = ms_since(t0, t1);

    t0 = std::clock(); simple::selection_sort(w1); t1 = std::clock(); double t_sel_worst = ms_since(t0, t1);
    t0 = std::clock(); simple::bubble_sort(w2);    t1 = std::clock(); double t_bub_worst = ms_since(t0, t1);
    t0 = std::clock(); simple::merge_sort(w3);     t1 = std::clock(); double t_mer_worst = ms_since(t0, t1);

    // CSV: таблица «Алгоритм/Случай/Время(сек)»
    csvout::save_sorting_cases_csv("sorting_cases.csv",
        t_sel_best,  t_bub_best,  t_mer_best,
        t_sel_avg,   t_bub_avg,   t_mer_avg,
        t_sel_worst, t_bub_worst, t_mer_worst);

    std::cout << "Созданы CSV: sorting.csv, search.csv, closest_pair.csv, sorting_cases.csv\n";
    std::cout << "\n=== Завершено успешно ===\n";
    return 0;
}
