/*
 * test_launchpad.cpp
 *
 *  Created on: Oct 12, 2024
 *      Author: mvo
 */

#ifndef LAUNCHPAD
#include "test_t.h"
#include "protection_t.h"
#include "state_machine_t.h"
#include "defines.h"
#include "LED_t.h"
#include "2_PWM.h"
#include "3_ADC.h"
#if (ALLOW_TEST == 1)
test_union_t test_t::cmd;
test_cmd_count_t test_t::count;
test_cmd_cfg_t test_t::cfg = {.LED_new_state = LED_OFF};
#if OPEN_LOOP_TESTING == 1
test_open_loop_t test_t::open_loop_test = {.p1_phi1 = 0, .p2_phi1 = 0, .p3_phi1 = 0, .p4_phi1 = 0};
#endif
#if FIXED_SW_COMB == 1
test_cell_t test_t::cell_test;
#endif

void test_t::init()
{
    // reset command request
    cmd.all = 0;
}

void test_t::background_routine()
{
    if (cmd.bit.start == 1)
    {
        cmd.bit.start = 0;
        count.start++;
        state_machine_t::start();
    }
    if (cmd.bit.stop == 1)
    {
        cmd.bit.stop = 0;
        count.stop++;
        state_machine_t::stop();
    }
    if (cmd.bit.clear_error == 1)
    {
        cmd.bit.clear_error = 0;
        state_machine_t::clear_errors();
        count.clear_error++;
    }
    if (cmd.bit.SW_error == 1)
    {
        cmd.bit.SW_error = 0;
        count.SW_error++;
        // set software error
        protection_t::set_error(SW_FORCE_ERROR);
    }
    // Set running requests on specific ports
    if (cmd.bit.port1_on == 1)
    {
        cmd.bit.port1_on = 0;
        count.port1_on++;
        state_machine_t::request_ports_to_operate(QAB_PORT1);
    }
    // Set running requests on specific ports
    if (cmd.bit.port2_on == 1)
    {
        cmd.bit.port2_on = 0;
        count.port2_on++;
        state_machine_t::request_ports_to_operate(QAB_PORT2);
    }
    // Set running requests on specific ports
    if (cmd.bit.port3_on == 1)
    {
        cmd.bit.port3_on = 0;
        count.port3_on++;
        state_machine_t::request_ports_to_operate(QAB_PORT3);
    }
    // Set running requests on specific ports
    if (cmd.bit.port4_on == 1)
    {
        cmd.bit.port4_on = 0;
        count.port4_on++;
        state_machine_t::request_ports_to_operate(QAB_PORT4);
    }
    // Remove running requests on specific ports
    if (cmd.bit.port1_off == 1)
    {
        cmd.bit.port1_off = 0;
        count.port1_off++;
        state_machine_t::request_ports_to_stop(QAB_PORT1);
    }
    // Remove running requests on specific ports
    if (cmd.bit.port2_off == 1)
    {
        cmd.bit.port2_off = 0;
        count.port2_off++;
        state_machine_t::request_ports_to_stop(QAB_PORT2);
    }
    // Remove running requests on specific ports
    if (cmd.bit.port3_off == 1)
    {
        cmd.bit.port3_off = 0;
        count.port3_off++;
        state_machine_t::request_ports_to_stop(QAB_PORT3);
    }
    // Remove running requests on specific ports
    if (cmd.bit.port4_off == 1)
    {
        cmd.bit.port4_off = 0;
        count.port4_off++;
        state_machine_t::request_ports_to_stop(QAB_PORT4);
    }

    if (cmd.bit.calibrate_v_sensors == 1)
    {
        cmd.bit.calibrate_v_sensors = 0;
        count.calibrate_v_sensors++;
        ADC_request.bit.calibrate_v = 1;
    }
#if (ALLOW_LED_TEST == 1)
    if (cmd.bit.LED1_new_state == 1)
    {
        cmd.bit.LED1_new_state = 0;
        count.LED1_new_state++;
        LED_t::set_LED_state(LED1, test_t::cfg.LED_new_state);
    }
    if (cmd.bit.LED2_new_state == 1)
    {
        cmd.bit.LED2_new_state = 0;
        count.LED2_new_state++;
        LED_t::set_LED_state(LED2, test_t::cfg.LED_new_state);
    }
    if (cmd.bit.LED3_new_state == 1)
    {
        cmd.bit.LED3_new_state = 0;
        count.LED3_new_state++;
        LED_t::set_LED_state(LED3, test_t::cfg.LED_new_state);
    }
    if (cmd.bit.LED4_new_state == 1)
    {
        cmd.bit.LED4_new_state = 0;
        count.LED4_new_state++;
        LED_t::set_LED_state(LED4, test_t::cfg.LED_new_state);
    }


    if (cmd.bit.LED1_off == 1)
    {
        cmd.bit.LED1_off = 0;
        count.LED1_off++;
        LED_t::set_LED_state(LED1, LED_OFF);
    }
    if (cmd.bit.LED2_off == 1)
    {
        cmd.bit.LED2_off = 0;
        count.LED2_off++;
        LED_t::set_LED_state(LED2, LED_OFF);
    }
    if (cmd.bit.LED3_off == 1)
    {
        cmd.bit.LED3_off = 0;
        count.LED3_off++;
        LED_t::set_LED_state(LED3, LED_OFF);
    }
    if (cmd.bit.LED4_off == 1)
    {
        cmd.bit.LED4_off = 0;
        count.LED4_off++;
        LED_t::set_LED_state(LED4, LED_OFF);
    }
#endif // ALLOW_LED_TEST == 1
#if (FIXED_SW_COMB == 1)
    if (cell_test.enable.cell1 == 1)
    {
        EnableEPwm4Gpio  ();       //Enable PWM4A and PWM4B - CELL 1
        EPwm_set_duty(&EPwm4Regs, cell_test.z.cell1);
    }
    else
    {
        DisableEPwm4Gpio  ();       //Disable PWM4A and PWM4B - CELL 1
    }
    if (cell_test.enable.cell2 == 1)
    {
        EnableEPwm3Gpio  ();       //Disable PWM3A and PWM3B - CELL 2
        EPwm_set_duty(&EPwm3Regs, cell_test.z.cell2);
    }
    else
    {
        DisableEPwm3Gpio  ();       //Disable PWM3A and PWM3B - CELL 2
    }
    if (cell_test.enable.cell3 == 1)
    {
        EnableEPwm6Gpio  ();       //Disable PWM6A and PWM6B - CELL 3
        EPwm_set_duty(&EPwm6Regs, cell_test.z.cell3);
    }
    else
    {
        DisableEPwm6Gpio  ();       //Disable PWM6A and PWM6B - CELL 3
    }
    if (cell_test.enable.cell4 == 1)
    {
        EnableEPwm5Gpio  ();       //Disable PWM5A and PWM5B - CELL 4
        EPwm_set_duty(&EPwm5Regs, cell_test.z.cell4);
    }
    else
    {
        DisableEPwm5Gpio  ();       //Disable PWM5A and PWM5B - CELL 4
    }


#endif
}


#endif // Allow_test == 1
#endif // LAUNCHPAD




