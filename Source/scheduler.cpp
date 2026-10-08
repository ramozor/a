/*
 * scheduler.c
 *
 *  Created on: 21. 8. 2017
 *      Author: kosan
 */

#include <stdint.h>
#include "F28x_Project.h"
#include "scheduler.h"

static task_t task_array[TASKS_MAX] = {NULL};

static uint16_t task_num = 0;
static volatile uint64_t g_sys_ticks_ms = 0; // make static to hide it from user as altering it can broke scheduler
static uint32_t idle_cnt = 0;
float cpu_load = 0; // number of SchedulerRun() executions in one tick of scheduler basic timing interrupt.
volatile uint16_t cpu_load_tmp = 0;

// Timer ISR
interrupt void SysTimerTask(void)
{
    // update system ticks
    g_sys_ticks_ms++;

    // update cpu_load_tmp
    cpu_load_tmp = idle_cnt;
    // clear idle cycles counter
    idle_cnt = 0;
    // ack interrupt group
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

// Scheduler routine
void SchedulerRun(void)
{
  uint16_t i;

  //TODO: test if it works also for different MCU frequencies
  cpu_load = 100.0 - (float)cpu_load_tmp*0.1;

  // count idle cycles
  idle_cnt++;


  for (i=0;i<task_num;i++)
  {
    // is valid pointer ? and is time to run code ?
    if (
            (task_array[i].task_fptr != NULL) &&
            ((g_sys_ticks_ms >= (task_array[i].timestamp + task_array[i].period)))// || (task_array[i].period == 0))
       )
    {
#if DEBUG_TIMING == 1
      task_array[i].ts_delta = g_sys_ticks_ms - task_array[i].timestamp - task_array[i].period;
      uint16_t task_start = ReadCpuTimer0Counter();
#endif
      if (task_array[i].options.bits.repetive_task == 1) // if the repetition is allowed
      {
          task_array[i].timestamp = g_sys_ticks_ms; // set last run timestamp
      }
      else
      {
          task_array[i].timestamp = ~((uint64_t)0)  - task_array[i].period; // Set last run timestamp close to inf.
      }
      task_array[i].task_fptr(); //run code
#if DEBUG_TIMING == 1
      task_array[i].elapsed_ns = 5*( abs(ReadCpuTimer0Counter() - task_start) + ((g_sys_ticks_ms - task_array[i].timestamp)*200000)); // *5 to get us
#endif
    }
  }
}

void SchedulerSetTaskTimestamp(task_t *task)
{
    task->timestamp = g_sys_ticks_ms;
}

void SchedulerSetTaskPeriod(task_t *task, uint32_t period)
{
    task->period = period;
    // reset the timing
    task->timestamp = g_sys_ticks_ms;
}

// add task with given period
task_t* SchedulerAddTask(task_fnc_ptr_t fptr, uint32_t period)
{
    if (task_num < TASKS_MAX)
    {
        if (period != 0)
        {
            task_array[task_num].period = period;
            task_array[task_num].timestamp = 0;
            task_array[task_num].options.bits.repetive_task = 1;
        }
        else
        {
            task_array[task_num].period = 0;
            task_array[task_num].timestamp = ((uint32_t)1)<<30;
            task_array[task_num].options.bits.repetive_task = 0;
        }
        task_array[task_num].task_fptr = fptr;
        task_array[task_num].task_time = 0;
        task_array[task_num].ts_delta = 0;
        return &task_array[task_num++];
    }
    return NULL;
}

// Scheduler init
void SchedulerInit(float cpu_freq, float tick_period)
{
    uint16_t i;
    // disable all tasks in array
    for (i=0;i<TASKS_MAX;i++)
    {
        task_array[i].task_fptr = NULL;
    }
    task_num = 0;

    // setup timers
    InitCpuTimers();
    // configure timer0
    ConfigCpuTimer(&CpuTimer0, cpu_freq, tick_period);
    // start timer
    StartCpuTimer0();
    EALLOW;
    PieVectTable.TIMER0_INT = &SysTimerTask;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT1;
    EDIS;
}

// Obtain timer stamp
//#pragma CODE_SECTION(".TI.ramfunc")
uint64_t SetTimerStamp(uint32_t len_ms)
{
    return g_sys_ticks_ms + len_ms;
}

// Check if a timer elapsed
//#pragma CODE_SECTION(".TI.ramfunc")
uint16_t CheckTimer(uint64_t stamp)
{
    // If timer elapsed, return 1 else return 0
    return (g_sys_ticks_ms >= stamp) ? 1 : 0;
}


/* EOF */
