#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

#include "../include/sorting_algorithms.h"
#include "../include/searching_algorithms.h"
#include "../include/closest_pair.h"
#include "../include/results_csv.h"

// ---------- генераторы ----------
static std::vector<int> make_array(int n, int seed) {
    std::srand(seed);
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = std::rand() % 200001 - 100000;
    return a;
}
static std::vector<int> make_best_case(int n) { std::vector<int> a(n); for (int i=0;i<n;++i)a[i]=i; return a; }
static std::vector<int> make_avg_case (int n, int seed) { return make_array(n, seed); }
static std::vector<int> make_worst_case(int n) { std::vector<int>a(n); for(int i=0;i<n;++i)a[i]=n-1-i; return a; }
static std::vector<simple::Point> make_points(int n, int seed) {
    std::srand(seed); std::vector<simple::Point> p(n);
    for (int i=0;i<n;++i){ p[i].x=(std::rand()%10000); p[i].y=(std::rand()%10000); p[i].id=i; }
    return p;
}
static double ms_since(std::clock_t t0, std::clock_t t1){ return 1000.0*(t1-t0)/CLOCKS_PER_SEC; }

// локальный бинарный поиск
static int binary_search_basic(const std::vector<int>& a, int key) {
    int L=0, R=(int)a.size()-1;
    while(L<=R){ int M=L+(R-L)/2; if(a[M]==key)return M; if(a[M]<key)L=M+1; else R=M-1; }
    return -1;
}

// короткий статус
static void print_sort_status(const char* name, const std::vector<int>& v, double ms) {
    std::cout << name << " | отсортировано=" << (std::is_sorted(v.begin(), v.end()) ? "да" : "нет")
              << " | время=" << ms << " мс\n";
}

int main() {
    std::cout << "Лабораторная работа 7–1: Сортировки, поиски, ближайшая пара\n\n";

    // ===== Сортировки (база) =====
    const int N = 2000;
    std::vector<int> base = make_array(N, 42);
    std::vector<int> a_sel = base, a_bub = base, a_mer = base;

    std::clock_t t0, t1;
    t0 = std::clock(); simple::selection_sort(a_sel); t1 = std::clock(); double ms_sel = ms_since(t0,t1);
    t0 = std::clock(); simple::bubble_sort(a_bub);    t1 = std::clock(); double ms_bub = ms_since(t0,t1);
    t0 = std::clock(); simple::merge_sort(a_mer);     t1 = std::clock(); double ms_mer = ms_since(t0,t1);

    std::cout << "- СОРТИРОВКИ -\n";
    print_sort_status("Выбором  ", a_sel, ms_sel);
    print_sort_status("Пузырьком", a_bub, ms_bub);
    print_sort_status("Слиянием ", a_mer, ms_mer);

    bool eq_mer_sel = (a_mer == a_sel);
    bool eq_bub_sel = (a_bub == a_sel);
    std::cout << "Совпадение (слияние vs выбор): " << (eq_mer_sel ? "да" : "нет") << "\n";
    std::cout << "Совпадение (пузырёк  vs выбор): " << (eq_bub_sel ? "да" : "нет") << "\n\n";

    csvout::save_sorting_csv("csv/sorting.csv", ms_sel, ms_bub, ms_mer, eq_mer_sel, eq_bub_sel);

    // ===== Поиски =====
    int key_present = base[N/3];
    int key_absent  = 123456789;

    int pos_seq_ok   = simple::sequential_search_sentinel(base, key_present);
    int pos_seq_miss = simple::sequential_search_sentinel(base, key_absent);

    std::vector<int> base_sorted = base; simple::merge_sort(base_sorted);
    int pos_bin_ok = binary_search_basic(base_sorted, key_present);

    std::cout << "- ПОИСК -\n";
    std::cout << "Последовательный: есть=" << pos_seq_ok << ", нет=" << pos_seq_miss << "\n";
    std::cout << "Бинарный (на отсорт.) : есть=" << pos_bin_ok << "\n\n";

    csvout::save_search_csv("csv/search.csv", pos_seq_ok, pos_seq_miss);

    // ===== Ближайшая пара =====
    const int M = 2000;
    std::vector<simple::Point> pts = make_points(M, 123);

    t0 = std::clock(); simple::CPResult bf = simple::closest_pair_bruteforce(pts); t1 = std::clock();
    double ms_bf = ms_since(t0,t1);
    t0 = std::clock(); simple::CPResult dc = simple::closest_pair_divide_conquer(pts); t1 = std::clock();
    double ms_dc = ms_since(t0,t1);

    std::cout << "- БЛИЖАЙШАЯ ПАРА -\n";
    std::cout << "Перебор: i=" << bf.i << " j=" << bf.j << " dist=" << bf.dist << " t=" << ms_bf << " мс\n";
    std::cout << "D&C   : i=" << dc.i << " j=" << dc.j << " dist=" << dc.dist << " t=" << ms_dc << " мс\n";

    csvout::save_closest_csv("csv/closest_pair.csv", bf, ms_bf, dc, ms_dc);

    // ===== Лучший/Средний/Худший =====
    const int N_cases = 1000;
    std::vector<int> best  = make_best_case(N_cases);
    std::vector<int> avg   = make_avg_case (N_cases, 2025);
    std::vector<int> worst = make_worst_case(N_cases);

    std::vector<int> b1 = best,  b2 = best,  b3 = best;
    std::vector<int> m1 = avg,   m2 = avg,   m3 = avg;
    std::vector<int> w1 = worst, w2 = worst, w3 = worst;

    double t_sel_best  = (t0=std::clock(), simple::selection_sort(b1), t1=std::clock(), ms_since(t0,t1));
    double t_bub_best  = (t0=std::clock(), simple::bubble_sort(b2),    t1=std::clock(), ms_since(t0,t1));
    double t_mer_best  = (t0=std::clock(), simple::merge_sort(b3),     t1=std::clock(), ms_since(t0,t1));

    double t_sel_avg   = (t0=std::clock(), simple::selection_sort(m1), t1=std::clock(), ms_since(t0,t1));
    double t_bub_avg   = (t0=std::clock(), simple::bubble_sort(m2),    t1=std::clock(), ms_since(t0,t1));
    double t_mer_avg   = (t0=std::clock(), simple::merge_sort(m3),     t1=std::clock(), ms_since(t0,t1));

    double t_sel_worst = (t0=std::clock(), simple::selection_sort(w1), t1=std::clock(), ms_since(t0,t1));
    double t_bub_worst = (t0=std::clock(), simple::bubble_sort(w2),    t1=std::clock(), ms_since(t0,t1));
    double t_mer_worst = (t0=std::clock(), simple::merge_sort(w3),     t1=std::clock(), ms_since(t0,t1));

    csvout::save_sorting_cases_csv("csv/sorting_cases.csv",
        t_sel_best,  t_bub_best,  t_mer_best,
        t_sel_avg,   t_bub_avg,   t_mer_avg,
        t_sel_worst, t_bub_worst, t_mer_worst);

    // ===== Серии по размерам (для графиков) =====
    {
        std::ofstream ofs("csv/sorting_sizes.csv", std::ios::out | std::ios::binary);
        // BOM
        const unsigned char bom[] = {0xEF,0xBB,0xBF}; ofs.write(reinterpret_cast<const char*>(bom),3);
        ofs.setf(std::ios::fixed); ofs << std::setprecision(9);
        ofs << "n;method;time_ms\n";
        const int sizes[] = {100,300,500,1000,2000,5000};
        for (int i=0;i<6;++i){
            int n=sizes[i];
            std::vector<int> s = make_array(n,777);
            std::vector<int> a=s,b=s,c=s;

            t0=std::clock(); simple::selection_sort(a); t1=std::clock(); ofs<<n<<";selection;"<<ms_since(t0,t1)<<"\n";
            t0=std::clock(); simple::bubble_sort(b);    t1=std::clock(); ofs<<n<<";bubble;"   <<ms_since(t0,t1)<<"\n";
            t0=std::clock(); simple::merge_sort(c);     t1=std::clock(); ofs<<n<<";merge;"    <<ms_since(t0,t1)<<"\n";
        }
    }
    {
        std::ofstream ofs("csv/search_sizes.csv", std::ios::out | std::ios::binary);
        const unsigned char bom[] = {0xEF,0xBB,0xBF}; ofs.write(reinterpret_cast<const char*>(bom),3);
        ofs.setf(std::ios::fixed); ofs << std::setprecision(9);
        ofs << "n;method;time_ms\n";
        const int sizes[] = {100,300,500,1000,2000,5000};
        for (int i=0;i<6;++i){
            int n=sizes[i];
            std::vector<int> d = make_array(n,888);
            int key = d[n/2];

            t0=std::clock(); (void)simple::sequential_search_sentinel(d,key); t1=std::clock();
            ofs<<n<<";sequential;"<<ms_since(t0,t1)<<"\n";

            std::vector<int> d_sorted = d; simple::merge_sort(d_sorted);
            t0=std::clock(); (void)binary_search_basic(d_sorted,key); t1=std::clock();
            ofs<<n<<";binary;"<<ms_since(t0,t1)<<"\n";
        }
    }

    std::cout << "CSV в папке ./csv: sorting.csv, sorting_cases.csv, sorting_sizes.csv, "
                 "search.csv, search_sizes.csv, closest_pair.csv\n";
    return 0;
}
