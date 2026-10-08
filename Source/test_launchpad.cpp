/*
 * test_launchpad.cpp
 *
 *  Created on: Oct 12, 2024
 *      Author: mvo
 */

#ifdef LAUNCHPAD
#include "test_t.h"
#include "protection_t.h"
#include "state_machine_t.h"
#include "defines.h"
#include "LED_t.h"
#if (ALLOW_TEST == 1)
test_union_t test_t::cmd;
test_cmd_count_t test_t::count;
test_cmd_cfg_t test_t::cfg = {.LED_new_state = LED_OFF};
#if FIXED_SW_COMB == 1
test_cell_duty test_t::z_test;
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
}


#endif // Allow_test == 1
#endif // LAUNCHPAD




