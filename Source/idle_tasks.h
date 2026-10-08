/**
 * @file idle_tasks.h
 * @brief Header file for managing idle task called by 1ms timer.
 * @date Oct 2024
 * @author Martin Votava
 **/




#ifndef SRC_IDLE_TASKS_H_
#define SRC_IDLE_TASKS_H_

/*!
 * Initialize basic idle tasks. It will setup all defined tasks, see idle_tasks.c.
 */
extern void IdleTasksInit(void);

#endif /* SRC_IDLE_TASKS_H_ */
