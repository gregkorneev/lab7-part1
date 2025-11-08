#pragma once
#include <string>
#include "../include/closest_pair.h"

namespace csvout {

// Базовые таблицы
void save_sorting_csv(const std::string& path,
                      double ms_selection,
                      double ms_bubble,
                      double ms_merge,
                      bool eq_merge_vs_selection,
                      bool eq_bubble_vs_selection);

void save_search_csv(const std::string& path,
                     int index_found,
                     int result_absent);

void save_closest_csv(const std::string& path,
                      const simple::CPResult& brute,
                      double ms_brute,
                      const simple::CPResult& divide_conquer,
                      double ms_dc);

// Таблица «Алгоритм / Случай / Время(сек)»
void save_sorting_cases_csv(const std::string& path,
                            double sel_best,  double bub_best,  double mer_best,
                            double sel_avg,   double bub_avg,   double mer_avg,
                            double sel_worst, double bub_worst, double mer_worst);

} // namespace csvout
