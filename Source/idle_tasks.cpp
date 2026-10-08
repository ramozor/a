/*
 * idle_tasks.c
 *
 *  Created on: 28. 7. 2017
 *      Author: kosan
 */


#include <stdint.h>
#include "idle_tasks.h"

#include "scheduler.h"
#include "LED_t.h"
#include "rumm_CAN.h"
#include "defines.h"
#include "globals.h" 
#include "can_debug.h"
//task_t idle_task_50ms, idle_task_1s = {.options.bits.repetive_task = 1}, idle_task_1ms, idle_task_heart_beat;


// 500 ms tasks
void IdleTask500ms(void)
{
    LED_t::routine_500ms();
#if ENABLE_CAN == 1
    //CAN_500ms_routine();
#endif
}

// 1 ms tasks
void IdleTask1ms(void)
{
   in.p1.meas.volt; 
   uint16_t target_var_id = 4;
   masterwrite(target_var_id, in.p1.meas.volt); 
    
}

  


// 50 ms task
void IdleTask50ms(void)
{
   

}

// 1 s task
void IdleTask1s(void)
{
    
    LED_t::routine_1s();
}

void IdleTasksInit(void)
{
  // setup tasks
  SchedulerAddTask(IdleTask1ms, 1);
  SchedulerAddTask(IdleTask50ms, 50);
  SchedulerAddTask(IdleTask1s, 1000);
  SchedulerAddTask(IdleTask500ms, 500);
}
