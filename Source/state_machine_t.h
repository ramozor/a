/**
 * @file state_machine_t.h
 * @brief State machine for controlling the operation modes of a converter system.
 *
 * This file defines the `state_machine_t` class, which manages the operational
 * states of the converter system, including initialization, reset, operation,
 * disabled mode, and error handling.
 *
 * @details
 * For detailed documentation, see @ref state_machine_docs "State Machine Documentation".
 * The reduced state machine uses the states `QAB_INIT`, `QAB_DISABLED`,
 * `QAB_RESET`, `QAB_OPERATION`, and `QAB_ERROR`. Intermediate states such as
 * isolation-switch initialization, charging, and turn-off have been removed from
 * the public state model. Their behavior, if still required, should be handled
 * inside the corresponding `.cpp` transition logic.
 *
 * @image html State_machine.png "State Machine Flowchart" width=33%
 * @note The state machine works in conjunction with protection mechanisms and the
 * control system to ensure safe operation of the converter.
 *
 * @author mvo
 * @date 10/2024
 */

#ifndef SOURCE_STATE_MACHINE_T_H_
#define SOURCE_STATE_MACHINE_T_H_

#include "scheduler.h"
#include "idle_tasks.h"
#include "protection_t.h"
#include <stdint.h>
#include "Defines.h"
#include "globals.h"

/**
 * @enum states_enum_t
 * @brief Enumeration for the reduced converter state machine.
 */
typedef enum
{
    QAB_INIT,       ///< Initial state.
    QAB_DISABLED,   ///< Converter disabled, outputs off.
    QAB_RESET,      ///< System reset state before operation.
    QAB_OPERATION,  ///< Normal operating state.
    // QAB_ERROR is kept last as the terminal/highest-priority fault state.
    QAB_ERROR,      ///< Error state.
} states_enum_t;

/**
 * @enum ports_qab_t
 * @brief Enumeration of QAB ports.
 */
typedef enum
{
    QAB_PORT1 = 1 << 0,
    QAB_PORT2 = 1 << 1,
    QAB_PORT3 = 1 << 2,
    QAB_PORT4 = 1 << 3,
} ports_qab_t;

/**
 * @union states_specification_t
 * @brief Union to store the current state of the system.
 */
typedef union
{
    states_enum_t label; ///< Labelled state using the enum.
    uint16_t value;      ///< Raw state value.
} states_specification_t;

/**
 * @typedef states_t
 * @brief Current state and its associated background routine.
 */
typedef struct
{
    states_specification_t state_spec;
    void (*background_routine_ptr)();
} states_t;

/**
 * @typedef port_enable_rq_bit_t
 * @brief Bitfield indicating which ports are requested to run.
 */
typedef struct
{
    uint16_t port1 : 1;
    uint16_t port2 : 1;
    uint16_t port3 : 1;
    uint16_t port4 : 1;
} port_enable_rq_bit_t;

/**
 * @typedef port_enable_rq_t
 * @brief Union indicating which ports are requested to run.
 */
typedef union
{
    uint16_t all;
    port_enable_rq_bit_t bit;
} port_enable_rq_t;

/**
 * @class state_machine_t
 * @brief Manages converter state transitions and state routines.
 */
class state_machine_t
{
public:
    /**
     * @brief Initializes the state machine and sets the initial state.
     */
    static void init();

    /**
     * @brief Runs the background routine associated with the current state.
     */
    static void background_routine();

    /**
     * @brief Attempts to start the converter system.
     *
     * Transition behavior is implemented in the `.cpp` file. In the reduced
     * state model, startup may move directly from disabled/reset handling into
     * operation if the required conditions are met.
     *
     * @return 1 if successful, 0 otherwise.
     */
    static uint16_t start();

    /**
     * @brief Runs the state-machine routine.
     */
    static void routine();

    /**
     * @brief Transitions the system into an error state and stops the converter.
     */
    static void stop_error();

    /**
     * @brief Stops the converter and transitions to the disabled state.
     */
    static void stop();

    /**
     * @brief Clears errors and sets the system to the disabled state.
     */
    static void clear_errors();

    inline static states_t get_state()
    {
        return state;
    }

    inline static void request_ports_to_operate(port_enable_rq_t ports_in)
    {
        port_rq.all |= ports_in.all;
    }

    inline static void request_ports_to_operate(uint16_t ports_in)
    {
        port_rq.all = ports_in;
    }

    inline static void request_ports_to_stop(port_enable_rq_t ports_in)
    {
        port_rq.all &= ~ports_in.all;
    }

    inline static void request_ports_to_stop(uint16_t ports_in)
    {
        port_rq.all &= ~ports_in;
    }

private:
    /// Current state of the system.
    static states_t state;

    /// Prevent instantiation.
    state_machine_t() {}

    /// Timed task for delayed transition logic.
    static task_t *timing_task;

    /// Ports requested to operate.
    static port_enable_rq_t port_rq;

    /// Variables for PHiL handling.
    static PHiL_handling_t phil_handle;

    /**
     * @brief Background routine for the reset state.
     */
    static void background_routine_system_in_reset();

    /**
     * @brief Background routine for the operating state.
     */
    static void background_routine_system_operating();

    /**
     * @brief Background routine for the error state.
     */
    static void background_routine_system_in_error();

    /**
     * @brief Background routine for the disabled state.
     */
    static void background_routine_system_disabled();

    /**
     * @brief Transitions from reset to the disabled state.
     */
    static void reset_to_disabled();

    inline static void set_qab_init_state()
    {
        state.background_routine_ptr = &background_routine_system_disabled;
        state.state_spec.label = QAB_INIT;
    }

    inline static void set_qab_disabled_state()
    {
        state.background_routine_ptr = &background_routine_system_disabled;
        state.state_spec.label = QAB_DISABLED;
    }

    inline static void set_qab_reset_state()
    {
        state.background_routine_ptr = &background_routine_system_in_reset;
        state.state_spec.label = QAB_RESET;
    }

    inline static void set_qab_operation_state()
    {
        state.background_routine_ptr = &background_routine_system_operating;

#if (EXP_SETUP == FAULT_CONTROL)
        // ctrl.set_phi1_limit(MAX_PHI1_CHARGING_DEG / 180.0f * M_PI);
#else
        ctrl.set_phi1_limit(MAX_PHI1_DEG / 180.0f * M_PI);
#endif

        state.state_spec.label = QAB_OPERATION;
    }

    inline static void set_qab_error_state()
    {
        state.background_routine_ptr = &background_routine_system_in_error;
        state.state_spec.label = QAB_ERROR;
    }

    inline static void delayed_execution(void (*fcn)(), uint16_t remaining_time)
    {
        timing_task->task_fptr = fcn;
        SchedulerSetTaskPeriod(timing_task, remaining_time);
    }

    inline static void unmask_undervoltage()
    {
        error_t error_mask_in = {.all = UNDERVOLTAGE_P1 | UNDERVOLTAGE_P2 |
                                        UNDERVOLTAGE_P3 | UNDERVOLTAGE_P4};
        protection_t::remove_mask(error_mask_in);
    }

    inline static void mask_undervoltage()
    {
        error_t error_mask_in = {.all = UNDERVOLTAGE_P1 | UNDERVOLTAGE_P2 |
                                        UNDERVOLTAGE_P3 | UNDERVOLTAGE_P4};
        protection_t::set_mask(error_mask_in);
    }

    static void reset_to_operation();
};

#endif /* SOURCE_STATE_MACHINE_T_H_ */
