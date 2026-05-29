#include "PercolationStats.h"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char ** argv)
{
    size_t dimension = 20;
    size_t trials = 100;

    if (argc >= 3) {
        dimension = atoi(argv[1]);
        trials = atoi(argv[2]);
    }

    PercolationStats stats(dimension, trials);
    stats.execute();

    cout << "mean                    = " << stats.get_mean() << endl;
    cout << "stddev                  = " << stats.get_standard_deviation() << endl;
    cout << "95% confidence interval = [" << stats.get_confidence_low()
         << ", " << stats.get_confidence_high() << "]" << endl;

    return 0;
}
