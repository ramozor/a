/**
 * @file      scheduler.h
 * @brief     Simple scheduler (header file).
 *
 * This file contains the declarations for a simple task scheduler. It provides
 * functionalities to add, configure, and run scheduled tasks based on system ticks.
 * For detailed documentation about the scheduler and its functionality,
 * click see more or refer to @ref scheduler_docs "Scheduler Documentation".
 *
 * @author    Martin Votava
 * @date      October 2024
 * @version   2.0
 */

#ifndef SHARED_SRC_SCHEDULER_H_
#define SHARED_SRC_SCHEDULER_H_

#include <stdint.h>

/** @def TASKS_MAX
 *  @brief Defines the maximum number of tasks that can be scheduled.
 */
#define TASKS_MAX (8)

/** @def DEBUG_TIMING
 *  @brief Set to 1 to enable timing measurements for debugging purposes.
 */
#define DEBUG_TIMING (1)

/**
 * @typedef task_fnc_ptr_t
 * @brief Function pointer type for task functions.
 *
 * This typedef defines a pointer to a function that returns void and takes
 * no arguments, representing a task in the scheduler.
 */
typedef void (*task_fnc_ptr_t)(void);

/**
 * @struct task_option_bits_t
 * @brief Structure defining task option bits.
 *
 * This structure holds the option bits for configuring task behavior,
 * such as whether the task should be repetitive.
 *
 * @var task_option_bits_t::repetive_task
 * Indicates whether the task is repetitive.
 * A value of 1 means the task will be scheduled repeatedly,
 * while 0 means it will run only once.
 */

/**
 * @typedef task_option_bits_t
 * @brief Typedef for the task option bits structure.
 *
 * This typedef is used to refer to the task_option_bits_t structure, which
 * configures whether a task is repetitive or one-time.
 */
typedef struct task_option_bits_t
{
    uint16_t repetive_task:1; /**< Indicates if the task is repetitive (1 = repetitive, 0 = one-time). */
} task_option_bits_t;

/**
 * @union task_options_union_t
 * @brief Union for task options.
 *
 * This union allows access to task options either as individual bits
 * or as a full 16-bit value.
 *
 * @var task_options_union_t::bits
 * Access individual bits of the task options.
 * @var task_options_union_t::all
 * Access all task options as a single 16-bit value.
 */

/**
 * @typedef task_options_union_t
 * @brief Typedef for task options union.
 *
 * This typedef is used to refer to the task_options_union_t union, which
 * allows access to task options as either individual bits or as a full 16-bit value.
 */
typedef union task_options_union_t {
    uint16_t all; /**< All task options as a single 16-bit value. */
    task_option_bits_t bits; /**< Task options as individual bits. */
} task_options_union_t;

/**
 * @struct task_t
 * @brief Structure defining a scheduled task.
 *
 * This structure holds information about each task, including the function
 * to run, the task's period, and timing-related data.
 */
typedef struct {
    uint32_t  timestamp;  /*!< Last run timestamp for internal use. */
    uint32_t  period;     /*!< The period (in system ticks) between task executions. */
    uint32_t  task_time;  /*!< The time spent in the task. */
    task_fnc_ptr_t task_fptr;   /*!< Pointer to the task function to be executed. */
    task_options_union_t options; /*!< Task options (e.g., whether the task is repetitive). */
#if DEBUG_TIMING == 1
    uint32_t elapsed_ns;  /*!< Elapsed time in scheduler ticks. */
    uint32_t ts_delta;    /*!< Delay of task execution (timestamp delta). */
#endif
} task_t;

/**
 * @brief Runs the scheduler and executes due tasks.
 *
 * This function should be called periodically. It checks system ticks
 * and executes tasks that are scheduled to run.
 */
void SchedulerRun(void);

/**
 * @brief Initializes the scheduler.
 *
 * This function sets up the hardware (TIMER0) required for the scheduler.
 * It must be called before any tasks can be added to the scheduler.
 *
 * @param cpu_freq     The CPU frequency in MHz.
 * @param tick_period  The basic scheduler period in microseconds.
 */
void SchedulerInit(float cpu_freq, float tick_period);

/**
 * @brief Adds a task to the scheduler.
 *
 * This function adds a task to the scheduler, which will call the provided
 * function at the specified period (in scheduler ticks).
 *
 * @param fptr   Pointer to the function to be called as a task.
 * @param period Period of function calls in scheduler ticks.
 * @return Pointer to the added task, or NULL if the task could not be added.
 */
task_t* SchedulerAddTask(task_fnc_ptr_t fptr, uint32_t period);

/**
 * @brief Checks if the timer has elapsed.
 *
 * This function checks whether the specified timer stamp has elapsed.
 *
 * @param stamp Timer stamp to check against.
 * @return 1 if the timer has elapsed, 0 otherwise.
 */
extern uint16_t CheckTimer(uint64_t stamp);

/**
 * @brief Sets a new timer stamp.
 *
 * This function sets a new timer stamp, which is the current system ticks
 * plus the specified duration in milliseconds.
 *
 * @param len_ms Length of the timer in milliseconds.
 * @return The new timer stamp.
 */
extern uint64_t SetTimerStamp(uint32_t len_ms);

/**
 * @brief Sets the timestamp for a task.
 *
 * This function updates the timestamp of a task to the current system ticks.
 *
 * @param task Pointer to the task whose timestamp is to be updated.
 */
extern void SchedulerSetTaskTimestamp(task_t *task);

/**
 * @brief Sets the period for a task.
 *
 * This function sets a new period for a task and resets its timestamp.
 *
 * @param task    Pointer to the task whose period is to be set.
 * @param period  The new period in scheduler ticks.
 */
extern void SchedulerSetTaskPeriod(task_t *task, uint32_t period);

#endif /* SHARED_SRC_SCHEDULER_H_ */
