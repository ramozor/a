/*
 * state_machine.cpp
 *
 *  Created on: Oct 9, 2024
 *      Author: mvo
 */

#include "state_machine_t.h"
#include "protection_t.h"

#include "defines.h"
#include "2_PWM.h"
#if (PORT_OPTION_SEL == PORTS_SEP)
#include "ctrl_var/ctrl_t.h"
#endif

#include "globals.h"

#include <stdlib.h>

// TODO: make new file with defines

#define EN_CELL1 GpioDataRegs.GPBSET.bit.GPIO39
#define DIS_CELL1 GpioDataRegs.GPBCLEAR.bit.GPIO39

#define EN_CELL2 GpioDataRegs.GPBSET.bit.GPIO55
#define DIS_CELL2 GpioDataRegs.GPBCLEAR.bit.GPIO55

PHiL_handling_t state_machine_t::phil_handle;

task_t* state_machine_t::timing_task;

states_t state_machine_t::state = {
    {QAB_INIT},
    background_routine_system_disabled
};

port_enable_rq_t state_machine_t::port_rq;

void state_machine_t::init()
{
    mask_undervoltage();
    set_qab_init_state();

    timing_task = SchedulerAddTask(background_routine_system_disabled, 0);
    set_qab_disabled_state();

#if (REQUIRED_ALL_PORT_OPERATION_AT_THE_BEGINNING == 1)
    port_rq.all = QAB_PORT1 | QAB_PORT2 | QAB_PORT3 | QAB_PORT4;
#endif
}

void state_machine_t::background_routine()
{
    state.background_routine_ptr();
}

uint16_t state_machine_t::start()
{
    in.p1.req = in.p3.req;
    in.p2.req = in.p3.req;
    in.p4.req = in.p3.req;

    if (!protection_t::get_error().all && (state.state_spec.label == QAB_DISABLED))
    {
        set_qab_reset_state();
        background_routine_system_in_reset();

        delayed_execution(reset_to_operation, 2000);
        return 1;
    }

    return 0;
}

void state_machine_t::stop_error()
{
    mask_undervoltage();

    EN2_OFF = 1;
    EN3_OFF = 1;
    EN4_OFF = 1;
    EN5_OFF = 1;
    EN6_OFF = 1;
    EN7_OFF = 1;
    EN8_OFF = 1;

    DIS_CELL1 = 1;
    DIS_CELL2 = 1;

    DisableEPwm2Gpio();
    DisableEPwm3Gpio();
    DisableEPwm4Gpio();
    DisableEPwm5Gpio();
    DisableEPwm6Gpio();
    DisableEPwm7Gpio();
    DisableEPwm8Gpio();

    set_qab_error_state();
    background_routine_system_in_error();
}

void state_machine_t::stop()
{
    mask_undervoltage();

    EN2_OFF = 1;
    EN3_OFF = 1;
    EN4_OFF = 1;
    EN5_OFF = 1;
    EN6_OFF = 1;
    EN7_OFF = 1;
    EN8_OFF = 1;

    DIS_CELL1 = 1;
    DIS_CELL2 = 1;

    DisableEPwm2Gpio();
    DisableEPwm3Gpio();
    DisableEPwm4Gpio();
    DisableEPwm5Gpio();
    DisableEPwm6Gpio();
    DisableEPwm7Gpio();
    DisableEPwm8Gpio();

    set_qab_disabled_state();
    background_routine_system_disabled();
}

void state_machine_t::clear_errors()
{
    if (state.state_spec.label == QAB_ERROR)
    {
        background_routine_system_in_reset();
        set_qab_reset_state();
        delayed_execution(reset_to_disabled, 2000);
    }
}

void state_machine_t::reset_to_operation()
{
    if (!protection_t::get_error().all && (state.state_spec.label == QAB_RESET))
    {
        ctrl.enable();
        set_qab_operation_state();
        background_routine_system_operating();
        delayed_execution(unmask_undervoltage, 1000);
    }
    else
    {
        set_qab_error_state();
        background_routine_system_in_error();
    }
}

void state_machine_t::reset_to_disabled()
{
    EPwm_clear_trip_zone();
    protection_t::clear_errors();

    if (!protection_t::get_error().all && (state.state_spec.label == QAB_RESET))
    {
        set_qab_disabled_state();
        background_routine_system_disabled();
    }
    else
    {
        set_qab_error_state();
        background_routine_system_in_error();
    }
}

void state_machine_t::background_routine_system_disabled()
{
    RST1_OFF = 1;
    RST2_OFF = 1;
    RST3_OFF = 1;
    RST4_OFF = 1;
    // PWM enable
    // Cell 1
    EN8_OFF = 1;
    EN7_OFF = 1;
    EN6_OFF = 1;
    EN5_OFF = 1;
    EN2_OFF = 1;
    EN3_OFF = 1;
    EN4_OFF = 1;
    EN5_OFF = 1;
    EN6_OFF = 1;

    DIS_CELL1 = 1;
    DIS_CELL2 = 1;

    
    DisableEPwm2Gpio();
    DisableEPwm3Gpio();
    DisableEPwm4Gpio();
    DisableEPwm5Gpio();
    DisableEPwm6Gpio();
    DisableEPwm7Gpio();
    DisableEPwm8Gpio();
    
}

void state_machine_t::background_routine_system_in_reset()
{
    RST1_ON = 1;
    RST2_ON = 1;
    RST3_ON = 1;
    RST4_ON = 1;

    EN2_OFF = 1;
    EN3_OFF = 1;
    EN4_OFF = 1;
    EN5_OFF = 1;
    EN6_OFF = 1;
    EN7_OFF = 1;
    EN8_OFF = 1;

    DIS_CELL1 = 1;
    DIS_CELL2 = 1;

    DisableEPwm2Gpio();
    DisableEPwm3Gpio();
    DisableEPwm4Gpio();
    DisableEPwm5Gpio();
    DisableEPwm6Gpio();
    DisableEPwm7Gpio();
    DisableEPwm8Gpio();
}

void state_machine_t::background_routine_system_operating()
{
    RST1_OFF = 1;
    RST2_OFF = 1;
    RST3_OFF = 1;
    RST4_OFF = 1;

    EN_CELL1 = 1;
    EN_CELL2 = 1;

    EN2_ON = 1;
    EN3_ON = 1;
    EN4_ON = 1;
    EN5_ON = 1;
    EN6_ON = 1;
    EN7_ON = 1;
    EN8_ON = 1;

    

    EnableEPwm2Gpio();
    EnableEPwm3Gpio();
    EnableEPwm4Gpio();
    EnableEPwm5Gpio();
    EnableEPwm6Gpio();
    EnableEPwm7Gpio();
    EnableEPwm8Gpio();
}

void state_machine_t::background_routine_system_in_error()
{
    EN2_OFF = 1;
    EN3_OFF = 1;
    EN4_OFF = 1;
    EN5_OFF = 1;
    EN6_OFF = 1;
    EN7_OFF = 1;
    EN8_OFF = 1;

    DIS_CELL1 = 1;
    DIS_CELL2 = 1;

    DisableEPwm2Gpio();
    DisableEPwm3Gpio();
    DisableEPwm4Gpio();
    DisableEPwm5Gpio();
    DisableEPwm6Gpio();
    DisableEPwm7Gpio();
    DisableEPwm8Gpio();
}

void state_machine_t::routine()
{
    
}
