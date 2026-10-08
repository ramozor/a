/*
 * globals.cpp
 *
 *  Created on: 24 Jun 2024
 *      Author: mvo
 */


#include "globals.h"


#if (EXP_SETUP == PHIL_EMULATION)
ctrl_PHiL_single_point_decoupling_t ctrl;
#endif
#if (EXP_SETUP == FAULT_CONTROL)
ctrl_fault_cond_t ctrl;
#endif
#if (!EXP_SETUP)
ctrl_single_point_decoupling_t ctrl;
#endif
ctrl_inputs_t in;
ctrl_outputs_t out;

float v1_filt = 0;


meas_filt_all_t meas_filt_100ms;
meas_filt_all_t meas_filt_10ms;

FLOAT_PREC volatile debug_grad[5];


