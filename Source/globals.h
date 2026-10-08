/**
 * @file globals.h
 * @brief Header file for managing global variables
 * @date Oct 2024
 * @author Martin Votava
 **/

#ifndef GLOBALS_H_
#define GLOBALS_H_
#include <stdint.h>

#include "type_definitions.h"

#include "Defines.h"
#if (PARALLEL_GRID_SUPPLY == 1)
#include "ctrl_var/no_fault/ctrl_parallel_con_t.h"
#else
#if (EXP_SETUP == PHIL_EMULATION)
#include "ctrl_var/phil/ctrl_PHiL_single_point_decoupling_t.h"
#endif
#if (EXP_SETUP == FAULT_CONTROL)
#include "ctrl_var/fault/ctrl_fault_cond_t.h"
#endif
#if (!EXP_SETUP)
#include "ctrl_var/no_fault/ctrl_single_point_decoupling_t.h"
#endif
#endif


#if (PARALLEL_GRID_SUPPLY == 1)
#else
#if (EXP_SETUP == PHIL_EMULATION)
extern ctrl_PHiL_single_point_decoupling_t ctrl;
#endif
#if (EXP_SETUP == FAULT_CONTROL)
extern ctrl_fault_cond_t ctrl;
#endif
#if (!EXP_SETUP)
extern ctrl_single_point_decoupling_t ctrl;
#endif
#endif

typedef struct
{
    port_meas_in_t p1;
    port_meas_in_t p2;
    port_meas_in_t p3;
    port_meas_in_t p4;
}meas_filt_all_t;



typedef struct
{
    FLOAT_PREC start_threshold_cur_p2; ///< Threshold current to start charging - port 2
    FLOAT_PREC start_threshold_cur_p4; ///< Threshold current to start charging - port 4
    FLOAT_PREC max_bat_volt; ///< Maximum voltage allowed voltage on the battery
    FLOAT_PREC min_bat_current_p2; ///< Minimum current to continue charging at the port 2
    FLOAT_PREC min_bat_current_p4; ///< Minimum current to continue charging at the port 4
    FLOAT_PREC max_bat_current_p2; ///< Maximum battery charging current at the port 2 - as the current reference value
    FLOAT_PREC max_bat_current_p4; ///< Maximum battery charging current at the port 4 - as the current reference value

}PHiL_handling_charging_t;

typedef struct
{
    FLOAT_PREC stop_dif_volt; ///< Threshold to stop the converter - difference between 10 and 100 ms filter
}PHiL_handling_turn_off_t;

typedef struct
{
    PHiL_handling_charging_t charging; ///< Charging thresholds
    PHiL_handling_turn_off_t stop; ///< Stoppage thresholds
}PHiL_handling_t;

extern float v1_filt;
extern ctrl_inputs_t in;
extern ctrl_outputs_t out;

extern meas_filt_all_t meas_filt_100ms;
extern meas_filt_all_t meas_filt_10ms;

extern volatile FLOAT_PREC debug_grad[5];








#endif /* GLOBALS_H_ */
