#include "PercolationStats.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t dimension = 0;
    size_t trials = 0;

    try {
        if (argc == 3) {
            dimension = stoull(argv[1]);
            trials = stoull(argv[2]);
        } else if (!(cin >> dimension >> trials)) {
            cerr << "Usage: percolation <dimension> <trials>\n";
            return 1;
        }

        PercolationStats stats(dimension, trials);
        stats.execute();

        cout << fixed << setprecision(6);
        cout << "mean = " << stats.get_mean() << '\n';
        cout << "stddev = " << stats.get_standard_deviation() << '\n';
        cout << "95% confidence interval = ["
             << stats.get_confidence_low() << ", "
             << stats.get_confidence_high() << "]\n";
    } catch (const exception& e) {
        cerr << e.what() << '\n';
        return 1;
    }
}
