#ifndef ESTIMATOR_T_H_
#define ESTIMATOR_T_H_

#include "type_definitions.h"
#include "cauer_problem_t.h"
#include "solver_t.h"

class estimator_t {
public:
    void init();
    void update_measurements(FLOAT_PREC u, FLOAT_PREC y);
    void run_optimization();
    void get_best_parameters(FLOAT_PREC* params);

    cauer_problem_t problem;
    solver_t<cauer_problem_t, true> solver;
    FLOAT_PREC current_best[5];
    uint32_t data_count = 0;
};

#endif