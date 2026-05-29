#include <iostream>
#include "..//include//percolation.hpp"

int main() {
    size_t dimension = 20;
    size_t trials = 100;

    std::cout << "Запуск " << trials << "экспериментов на сетке " << dimension << "x" << dimension << std::endl;

    PercolationStats stats(dimension, trials);
    stats.execute();

    std::cout << "Среднее значение =  " << stats.get_mean() << std::endl;
    std::cout << "Стандартное отклонение = " << stats.get_standard_deviation() << std::endl;
    std::cout << "Доверительный интервал (95%) = [" 
              << stats.get_confidence_low() << ", " 
              << stats.get_confidence_high() << "]" << std::endl;

    return 0;
}