/*
 * CSV-вывод с поддержкой кириллицы (UTF-8 с BOM) и фиксированной точностью.
 * Теперь Excel открывает русские буквы правильно.
 */

#include "../include/results_csv.h"
#include <fstream>
#include <string>
#include <iomanip>   // std::fixed, std::setprecision

namespace csvout {

// Открыть CSV-файл в UTF-8 с BOM, записать заголовок и задать формат вывода
static bool open_with_header(const std::string& path,
                             const std::string& header,
                             std::ofstream& ofs)
{
    ofs.open(path, std::ios::out | std::ios::binary);
    if (!ofs.is_open()) return false;

    // Добавляем BOM для Excel
    const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    ofs.write(reinterpret_cast<const char*>(bom), sizeof(bom));

    // Настройка чисел: фиксированный формат, 9 знаков после запятой
    ofs.setf(std::ios::fixed);
    ofs << std::setprecision(9);

    ofs << header << "\n";
    return true;
}

// ---- sorting.csv ----
void save_sorting_csv(const std::string& path,
                      double ms_selection,
                      double ms_bubble,
                      double ms_merge,
                      bool eq_merge_vs_selection,
                      bool eq_bubble_vs_selection)
{
    std::ofstream ofs;
    if (!open_with_header(path, "method;time_ms;equal_to_selection", ofs)) return;

    ofs << "SelectionSort;" << ms_selection << ";yes\n";
    ofs << "BubbleSort;"    << ms_bubble    << ";" << (eq_bubble_vs_selection ? "yes" : "no") << "\n";
    ofs << "MergeSort;"     << ms_merge     << ";" << (eq_merge_vs_selection  ? "yes" : "no") << "\n";
}

// ---- search.csv ----
void save_search_csv(const std::string& path,
                     int index_found,
                     int result_absent)
{
    std::ofstream ofs;
    if (!open_with_header(path, "key_state;result", ofs)) return;

    ofs << "present;" << index_found   << "\n";
    ofs << "absent;"  << result_absent << "\n";
}

// ---- closest_pair.csv ----
void save_closest_csv(const std::string& path,
                      const simple::CPResult& brute,
                      double ms_brute,
                      const simple::CPResult& divide_conquer,
                      double ms_dc)
{
    std::ofstream ofs;
    if (!open_with_header(path, "method;i;j;distance;time_ms", ofs)) return;

    ofs << "Bruteforce;"     << brute.i << ";" << brute.j << ";" << brute.dist << ";" << ms_brute << "\n";
    ofs << "DivideConquer;"  << divide_conquer.i << ";" << divide_conquer.j
        << ";" << divide_conquer.dist << ";" << ms_dc << "\n";
}

// ---- sorting_cases.csv ----
void save_sorting_cases_csv(const std::string& path,
                            double sel_best,  double bub_best,  double mer_best,
                            double sel_avg,   double bub_avg,   double mer_avg,
                            double sel_worst, double bub_worst, double mer_worst)
{
    std::ofstream ofs;
    if (!open_with_header(path, "Algorithm;Case;Time_ms", ofs)) return;

    ofs << "SelectionSort;Лучший;"  << sel_best  << "\n";
    ofs << "BubbleSort;Лучший;"     << bub_best  << "\n";
    ofs << "MergeSort;Лучший;"      << mer_best  << "\n";

    ofs << "SelectionSort;Средний;" << sel_avg   << "\n";
    ofs << "BubbleSort;Средний;"    << bub_avg   << "\n";
    ofs << "MergeSort;Средний;"     << mer_avg   << "\n";

    ofs << "SelectionSort;Худший;"  << sel_worst << "\n";
    ofs << "BubbleSort;Худший;"     << bub_worst << "\n";
    ofs << "MergeSort;Худший;"      << mer_worst << "\n";
}

} // namespace csvout
