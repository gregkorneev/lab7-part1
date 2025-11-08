#include "../include/results_csv.h"
#include <fstream>
#include <string>
#include <iomanip>   // std::fixed, std::setprecision

namespace csvout {

// Открыть файл, записать заголовок и выставить формат чисел
static bool open_with_header(const std::string& path,
                             const std::string& header,
                             std::ofstream& ofs)
{
    ofs.open(path, std::ios::out);
    if (!ofs.is_open()) return false;

    // Числа всегда печатаем как fixed с 9 знаками
    ofs.setf(std::ios::fixed);
    ofs << std::setprecision(9);

    ofs << header << "\n";
    return true;
}

// sorting.csv: method;time_ms;equal_to_selection
void save_sorting_csv(const std::string& path,
                      double ms_selection,
                      double ms_bubble,
                      double ms_merge,
                      bool eq_merge_vs_selection,
                      bool eq_bubble_vs_selection)
{
    std::ofstream ofs;
    if (!open_with_header(path, "method;time_ms;equal_to_selection", ofs)) return;

    ofs << "selection;" << ms_selection << ";yes\n";
    ofs << "bubble;"    << ms_bubble    << ";" << (eq_bubble_vs_selection ? "yes" : "no") << "\n";
    ofs << "merge;"     << ms_merge     << ";" << (eq_merge_vs_selection  ? "yes" : "no") << "\n";
}

// search.csv: key_state;result
void save_search_csv(const std::string& path,
                     int index_found,
                     int result_absent)
{
    std::ofstream ofs;
    if (!open_with_header(path, "key_state;result", ofs)) return;

    ofs << "present;" << index_found   << "\n";
    ofs << "absent;"  << result_absent << "\n";
}

// closest_pair.csv: method;i;j;distance;time_ms
void save_closest_csv(const std::string& path,
                      const simple::CPResult& brute,
                      double ms_brute,
                      const simple::CPResult& divide_conquer,
                      double ms_dc)
{
    std::ofstream ofs;
    if (!open_with_header(path, "method;i;j;distance;time_ms", ofs)) return;

    ofs << "bruteforce;"     << brute.i << ";" << brute.j << ";" << brute.dist << ";" << ms_brute << "\n";
    ofs << "divide_conquer;" << divide_conquer.i << ";" << divide_conquer.j
        << ";" << divide_conquer.dist << ";" << ms_dc << "\n";
}

// sorting_cases.csv: algorithm;case;time_ms  (ТЕПЕРЬ пишем миллисекунды)
void save_sorting_cases_csv(const std::string& path,
                            double sel_best,  double bub_best,  double mer_best,
                            double sel_avg,   double bub_avg,   double mer_avg,
                            double sel_worst, double bub_worst, double mer_worst)
{
    std::ofstream ofs;
    if (!open_with_header(path, "algorithm;case;time_ms", ofs)) return;

    // Все значения уже в миллисекундах — пишем как есть
    ofs << "SelectionSort;Лучший;" << sel_best  << "\n";
    ofs << "BubbleSort;Лучший;"    << bub_best  << "\n";
    ofs << "MergeSort;Лучший;"     << mer_best  << "\n";

    ofs << "SelectionSort;Средний;"<< sel_avg   << "\n";
    ofs << "BubbleSort;Средний;"   << bub_avg   << "\n";
    ofs << "MergeSort;Средний;"    << mer_avg   << "\n";

    ofs << "SelectionSort;Худший;" << sel_worst << "\n";
    ofs << "BubbleSort;Худший;"    << bub_worst << "\n";
    ofs << "MergeSort;Худший;"     << mer_worst << "\n";
}

}
