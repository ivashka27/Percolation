#include "percolation.h"
#include "percolation_stats.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

static void test_percolation_simple_path()
{
    Percolation percolation(3);

    assert(!percolation.percolates());
    percolation.open(0, 1);
    assert(percolation.is_open(0, 1));
    assert(percolation.is_full(0, 1));
    assert(!percolation.percolates());

    percolation.open(1, 1);
    assert(percolation.is_full(1, 1));
    assert(!percolation.percolates());

    percolation.open(2, 1);
    assert(percolation.is_full(2, 1));
    assert(percolation.percolates());
    assert(percolation.get_open_sites() == 3);
}

static void test_percolation_blocked_cell_is_not_full()
{
    Percolation percolation(2);
    percolation.open(0, 0);

    assert(!percolation.is_open(1, 1));
    assert(!percolation.is_full(1, 1));
    assert(!percolation.percolates());
}

static void test_percolation_bad_arguments()
{
    bool got_dimension_error = false;
    try
    {
        Percolation percolation(0);
    }
    catch (const std::invalid_argument&)
    {
        got_dimension_error = true;
    }
    assert(got_dimension_error);

    bool got_coordinates_error = false;
    try
    {
        Percolation percolation(2);
        percolation.open(2, 0);
    }
    catch (const std::out_of_range&)
    {
        got_coordinates_error = true;
    }
    assert(got_coordinates_error);
}

static void test_stats_single_cell()
{
    PercolationStats stats(1, 5);
    stats.execute();

    assert(std::fabs(stats.get_mean() - 1.0) < 1e-12);
    assert(std::fabs(stats.get_standard_deviation()) < 1e-12);
    assert(std::fabs(stats.get_confidence_low() - 1.0) < 1e-12);
    assert(std::fabs(stats.get_confidence_high() - 1.0) < 1e-12);
}

static void test_stats_bad_arguments()
{
    bool got_dimension_error = false;
    try
    {
        PercolationStats stats(0, 1);
    }
    catch (const std::invalid_argument&)
    {
        got_dimension_error = true;
    }
    assert(got_dimension_error);

    bool got_trials_error = false;
    try
    {
        PercolationStats stats(2, 0);
    }
    catch (const std::invalid_argument&)
    {
        got_trials_error = true;
    }
    assert(got_trials_error);
}

int main()
{
    test_percolation_simple_path();
    test_percolation_blocked_cell_is_not_full();
    test_percolation_bad_arguments();
    test_stats_single_cell();
    test_stats_bad_arguments();

    std::cout << "All tests passed\n";
    return 0;
}
