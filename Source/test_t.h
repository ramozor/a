/**
 * @file test_t.h
 * @brief Header file for the test management system.
 *
 * This file defines the structures and functions used for managing and controlling
 * test operations, including command handling, LED state management, error tracking,
 * and testing routines. It provides an interface for issuing commands to start, stop,
 * or manipulate the system's LED states as well as tracking command counts and handling
 * errors during the testing process.
 *
 * The test system consists of bit fields for different test commands, unions for accessing
 * command bits, and static methods for initializing and running the test routines.
 *
 * @date Oct 2024
 * @author mvo
 */

#ifndef SOURCE_TEST_T_H_
#define SOURCE_TEST_T_H_

#include <stdint.h>
#include "LED_t.h"
#include "defines.h"

/**
 * @struct test_cell_duty_t
 * @brief Stores cell-specific values for PWM duty cycle calculations.
 *
 * This structure holds the input values for each cell (z_cell1 to z_cell4),
 * which are later used as inputs for functions like `EPwm_set_duty`.
 *
 * @var cell_test::z_cell1
 * Duty cycle value for Cell 1.
 * @var cell_test::z_cell2
 * Duty cycle value for Cell 2.
 * @var cell_test::z_cell3
 * Duty cycle value for Cell 3.
 * @var cell_test::z_cell4
 * Duty cycle value for Cell 4.
 */
typedef struct
{
    float cell1;
    float cell2;
    float cell3;
    float cell4;
}test_cell_duty_t;

typedef struct
{
    uint16_t cell1:1;
    uint16_t cell2:1;
    uint16_t cell3:1;
    uint16_t cell4:1;
}test_cell_enable_t;

typedef struct
{
    test_cell_duty_t z;
    test_cell_enable_t enable;
}test_cell_t;

typedef struct
{
    float p1_phi1;
    float p2_phi1;
    float p3_phi1;
    float p4_phi1;
    float p1_phi2;
    float p2_phi2;
    float p3_phi2;
    float p4_phi2;
    float A_rel;
}test_open_loop_t;

/**
 * @struct test_bit_t
 * @brief Structure that holds control bits for various operations.
 *
 * This structure contains bit fields that represent the states and commands
 * for starting, stopping, controlling LEDs, and handling errors.
 */
typedef struct
{
    uint32_t start:1;           /**< Start operation bit. */
    uint32_t stop:1;            /**< Stop operation bit. */
    uint32_t calibrate_v_sensors:1; /**< Calibrate voltage sensors during and reset the drivers - bit. */
    uint32_t LED1_new_state:1;  /**< LED1 new state bit. */
    uint32_t LED2_new_state:1;  /**< LED2 new state bit. */
    uint32_t LED3_new_state:1;  /**< LED3 new state bit. */
    uint32_t LED4_new_state:1;  /**< LED4 new state bit. */
    uint32_t LED1_off:1;        /**< LED1 off state bit. */
    uint32_t LED2_off:1;        /**< LED2 off state bit. */
    uint32_t LED3_off:1;        /**< LED3 off state bit. */
    uint32_t LED4_off:1;        /**< LED4 off state bit. */
    uint32_t port1_on;          /**< port 1 on commands. */
    uint32_t port2_on;          /**< port 2 on commands. */
    uint32_t port3_on;          /**< port 3 on commands. */
    uint32_t port4_on;          /**< port 4 on commands. */
    uint32_t port1_off;         /**< port 1 off commands. */
    uint32_t port2_off;         /**< port 2 off commands. */
    uint32_t port3_off;         /**< port 3 off commands. */
    uint32_t port4_off;         /**< port 4 off commands. */
    uint32_t SW_error:1;        /**< Software error cmd. */
    uint32_t clear_error:1;     /**< Clear error bit. */
} test_bit_t;

/**
 * @union test_union_t
 * @brief Union that allows access to test bits either as individual fields or as a whole.
 *
 * This union provides a way to access test control bits as either individual bit
 * fields or as a 16-bit value.
 */
typedef union
{
    test_bit_t bit;  /**< Individual bit fields. */
    uint32_t all;    /**< All bit fields as a 16-bit value. */
} test_union_t;

/**
 * @struct test_cmd_count_t
 * @brief Structure that counts the number of commands issued for various operations.
 *
 * This structure stores the counts of different commands issued, such as start, stop,
 * LED control, and error handling commands.
 */
typedef struct
{
    uint16_t start;           /**< Count of start commands. */
    uint16_t stop;            /**< Count of stop commands. */
    uint16_t calibrate_v_sensors; /*< Count of calibrate voltage commands. */
    uint16_t LED1_new_state;  /**< Count of new state commands for LED1. */
    uint16_t LED2_new_state;  /**< Count of new state commands for LED2. */
    uint16_t LED3_new_state;  /**< Count of new state commands for LED3. */
    uint16_t LED4_new_state;  /**< Count of new state commands for LED4. */
    uint16_t LED1_off;        /**< Count of off commands for LED1. */
    uint16_t LED2_off;        /**< Count of off commands for LED2. */
    uint16_t LED3_off;        /**< Count of off commands for LED3. */
    uint16_t LED4_off;        /**< Count of off commands for LED4. */
    uint16_t port1_on;        /**< Count of port 1 on commands. */
    uint16_t port2_on;        /**< Count of port 2 on commands. */
    uint16_t port3_on;        /**< Count of port 3 on commands. */
    uint16_t port4_on;        /**< Count of port 4 on commands. */
    uint16_t port1_off;       /**< Count of port 1 off commands. */
    uint16_t port2_off;       /**< Count of port 2 off commands. */
    uint16_t port3_off;       /**< Count of port 3 off commands. */
    uint16_t port4_off;       /**< Count of port 4 off commands. */
    uint16_t SW_error;        /**< Count of software errors. */
    uint16_t clear_error;     /**< Count of clear error commands. */

} test_cmd_count_t;

/**
 * @struct test_cmd_cfg_t
 * @brief Configuration structure for test commands related to LEDs.
 *
 * This structure contains the new state configurations for the LEDs.
 */
typedef struct
{
    LED_state_enum_t LED_new_state;  /**< New state for LEDs. */
} test_cmd_cfg_t;

#if (ALLOW_TEST == 1)

/**
 * @class test_t
 * @brief A class to handle the test operations.
 *
 * This class provides methods for initializing the test environment and running
 * background routines. It also contains static members to hold the command structure,
 * count structure, and configuration structure for tests.
 */
class test_t
{
public:
    static test_union_t cmd;        /**< Command structure for test operations. */
    static test_cmd_count_t count;  /**< Count structure for tracking commands. */
    static test_cmd_cfg_t cfg;      /**< Configuration structure for test settings. */
#if FIXED_SW_COMB == 1
    static test_cell_t cell_test;  /**< Struct for testing constant duty cycle on cells*/
#endif
#if OPEN_LOOP_TESTING == 1
    static test_open_loop_t open_loop_test; /**< Struct for testing a power converter with open loop control*/
#endif

    /**
     * @brief Initializes the test environment.
     */
    static void init();

    /**
     * @brief Runs the background routine for test operations.
     */
    static void background_routine();

private:
    // Private members or methods could be added here if necessary.
};
#endif

#endif /* SOURCE_TEST_H_ */
