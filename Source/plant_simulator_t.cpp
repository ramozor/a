#include "plant_simulator_t.h"

void plant_simulator_t::init() {
    state_space_var1_par_t real_sys_params;
    FLOAT_PREC real_R1 = 2.0, real_tau1 = 0.05;
    FLOAT_PREC real_R2 = 5.0, real_tau2 = 0.5;
    FLOAT_PREC sample_time = 5e-3;
    
    real_sys_params.a11 = 1.0 - sample_time / real_tau1;
    real_sys_params.a12 = sample_time / real_tau1;
    real_sys_params.a21 = (real_R2 * sample_time) / (real_R1 * real_tau2);
    real_sys_params.a22 = 1.0 - real_sys_params.a21 - sample_time / real_tau2;
    real_sys_params.b1  = (real_R1 * sample_time) / real_tau1;

    dummy_hardware.init(real_sys_params);
    dummy_hardware.reset();
    dummy_hardware.step(0); // İlk adım
}

void plant_simulator_t::step(uint32_t tick, FLOAT_PREC* out_u, FLOAT_PREC* out_y) {
    FLOAT_PREC angle_fast   = tick * 0.1;
    FLOAT_PREC angle_medium = tick * 0.02;
    FLOAT_PREC angle_slow   = tick * 0.005;

    FLOAT_PREC multi_sine = 0.20 * sin(angle_fast) + 0.15 * sin(angle_medium) + 0.15 * sin(angle_slow);
    FLOAT_PREC dummy_u = 1.0 + multi_sine;

    dummy_hardware.step(dummy_u);
    
    *out_u = dummy_u;
    *out_y = dummy_hardware.get_y();
}