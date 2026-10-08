
// #################################################################
//
//  FILE:    Main.c
//  TITLE:   Main Program v1.0
//  Author: Thiago Pereira - March 2020
//
//
//=================================================================
//  Includes
//=================================================================
#include "Defines.h"
#include "F28x_Project.h"
#include "General.h"
#include <math.h>


#include <stdbool.h> /* Includes true/false definition */
#include <stdint.h>  /* Includes uint16_t definition   */
#include <stdio.h>


#include "1_GPIO.h"
#include "2_PWM.h"
#include "3_ADC.h"
#include "5_ISR.h"
#include "rumm_can.h"
#include "can_debug.h"
#include "LED_t.h"
#include "peripheral.h"
#include "protection_t.h"
#include "scheduler.h"
#include "state_machine_t.h"
#include "test_t.h"

#include "globals.h"
#include "src_editable/ctrl_init.h"
#include "can_debug.h" 
#include "string.h"
#include "rumm_can.h"
#include "can_base.h"
#include "cauer_problem_t.h"
#include "solver_t.h"
#include "toy_problem_t.h"
#include "state_space_var1_t.h"
#include "estimator_t.h"
#include "plant_simulator_t.h"
// Enable PWM
// GPIO25 - PWM A,B
// GPIO24 - PWM C,D
// GPIO99 - PWM E,F
// GPIO86 - PWM G,H
// GPIO87 - PWM I,J
// GPIO88 - PWM K,L
// Cell 1:
// GPIO89 - PWM M,N
// GPIO90 - PWM O,P

// PWM A,B - GPIO0,1 - ePWM1
// ????
// PWM C,D - GPIO2,3 - ePWM2
// PWM E,F - GPIO4,5 - ePWM3
// PWM G,H - GPIO6,7 - ePWM4
// PWM I,J - GPIO8,9 - ePWM5
// PWM K,L - GPIO10,11 - ePWM6
// PWM M,N - GPIO12,13 - ePWM7
// PWM O,P - GPIO14,15 - ePWM8

// FAULT
// GPIO16 - PWM A ok
// GPIO18 - PWM B - ok
// GPIO20 - PWM C - ok
// GPIO21 - PWM D - ok
// GPIO22 - PWM E - ok
// GPIO23 - PWM F - ok


// External 
// 37 - left 1
// 39 - left 2
// 41 - left 3
// 55 - 4,5,6,7


//=================================================================
//
//
//
#ifdef _FLASH
//=================================================================
// These are defined by the linker (see device linker command file)
//=================================================================
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
#endif
//
//

// Toy optimization problem evaluated by the solver.
toy_problem_t toy_problem;

// Gradient-descent solver configured for toy_problem_t.
// The second template argument enables full unrolling of parameter loops.
//solver_t<toy_problem_t, true> solver;
cauer_problem_t cauer_problem;
// The second template argument enables full unrolling of parameter loops.
solver_t<cauer_problem_t, true> solver;

state_space_var1_t dummy_hardware;
estimator_t parameter_estimator;
plant_simulator_t plant_sim;

//=================================================================
// Main
//=================================================================
void do_nothing()
{
  return;
}
int main() {


  // Init DSP peripherals, GPIOs, and system clocks
  protection_t::stop_error = &do_nothing;
  peripheral_init();
  // software init procedure
  SchedulerInit(200, 1000); // Main task scheduling (reserves timer0)
  init_control();   // Init QAB control lib (parameters)

  state_machine_t::init(); // Init system state machine
#if ALLOW_TEST == 1
  test_t::init(); // Init testing class
#endif
  IdleTasksInit(); // Init scheduled tasks and add them to the scheduler

  // Initialize Interruption Routine after the software is initialized
  ISR();
  // Init toy problem
  parameter_estimator.init();
  plant_sim.init();
  parameter_estimator.update_measurements(0, 0);
  
  //current_best[5]= 
  

  //
  //
  //================================================================================================
  //-------------------------------------- Infinite Loop
  //-------------------------------------------
  //================================================================================================
  
  while (1) {
        static int loop_delay = 0;
        static uint32_t dummy_isr_tick = 0;
        loop_delay++;

        // -------------------------------------------------------------------------
        // A. INTERRUPT (ISR) SIMULATION (To be moved into the hardware Timer ISR in the future)
        // -------------------------------------------------------------------------
        if (loop_delay % 1000 == 0) {
            dummy_isr_tick++;

            FLOAT_PREC current_u, current_y;
            
            // Read the sensors and advance the physical system
            plant_sim.step(dummy_isr_tick, &current_u, &current_y);

            // Send the measurements to the estimator
            parameter_estimator.update_measurements(current_u, current_y);

            // Run the optimization
            parameter_estimator.run_optimization();
        }

    // -------------------------------------------------------------------------
    // B. RUNNING THE OPTIMIZATION (SOLVER)
    // We divide it into specific periods to avoid running the solver on every CPU cycle
    // -------------------------------------------------------------------------
    SchedulerRun();
    state_machine_t::background_routine();
#if ALLOW_TEST == 1
    test_t::background_routine();
#endif
    LED_t::background_routine();

    CanDebugProcessInput();
  } // End while(1)
  //
} // End void main(void)
