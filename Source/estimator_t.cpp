#include "estimator_t.h"

void estimator_t::init() {
    // Solver parametreleri
    solver_params_t par_solver = { .learning_rate = 0.01 };
    solver.init(&problem, par_solver);

    // Başlangıç tahminleri
    FLOAT_PREC initial_guess[5] = {5, 0.1, 2, 1, 0.001};
    for(int i=0; i<5; i++) current_best[i] = initial_guess[i];
    solver.set_initial_guess(current_best);

    // Cauer problemi parametreleri
    cauer_params_t par_cauer = {
        .y_data = {0},
        .u_data = {1},
        .Ts = 5e-3
    };
    problem.init(par_cauer);
    data_count = 0;
}

void estimator_t::update_measurements(FLOAT_PREC u, FLOAT_PREC y) {
    data_count++;

    // Tampon dolduğunda x2_hat başlangıç koşulunu kendi modelimizle güncelle
    if (data_count > N_DATA_POINTS-1) {
        solver.get_best_parameters(current_best);
        FLOAT_PREC est_R1   = current_best[0];
        FLOAT_PREC est_tau1 = current_best[1];
        FLOAT_PREC est_R2   = current_best[2];
        FLOAT_PREC est_tau2 = current_best[3];
        FLOAT_PREC sample_time = 5e-3; // Ts

        FLOAT_PREC est_a21 = (est_R2 * sample_time) / (est_R1 * est_tau2);
        FLOAT_PREC est_a22 = 1.0 - est_a21 - sample_time / est_tau2;

        current_best[4] = est_a21 * problem.get_the_oldest_y() + est_a22 * current_best[4];
    }
    
    problem.update_measurements(u, y);
}

void estimator_t::run_optimization() {
    // Yeterli veri biriktiyse optimizasyonu çalıştır
    if (data_count > N_DATA_POINTS - 2) {
        solver.set_initial_guess(current_best);
        problem.snapshot_for_cost();

        for (uint16_t i = 0; i < 50; i++) {
            solver.grad_calc();
        }
        solver.get_best_parameters(current_best);
    }
}

void estimator_t::get_best_parameters(FLOAT_PREC* params) {
    for(int i=0; i<5; i++) params[i] = current_best[i];
}